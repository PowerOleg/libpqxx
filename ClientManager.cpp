#include "ClientManager.h"
#include <iostream>
#include <tuple>

ClientManager::ClientManager(pqxx::connection&& conn) : connection { std::move(conn) }
{
	std::cout << "Connection is successful" << std::endl;
}

void ClientManager::create_table(std::string table, std::string param)
{
	try
	{
	pqxx::work tx{ this->connection };
	tx.exec("CREATE TABLE IF NOT EXISTS " + table + " (" + param + ");");
	tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void ClientManager::initDbStructure()
{
	create_table(
		"Client",
		"id SERIAL PRIMARY KEY, "
		"firstname varchar(255) NOT NULL, "
		"secondname varchar(255) NOT NULL"
	);
	std::cout << "A table Client has created" << std::endl;
	create_table(
		"ClientsData",
		"id SERIAL PRIMARY KEY, "
		"email varchar(255), "
		"phone_number varchar(255), "
		"client_id int references Client(id)"
	);
	std::cout << "A table ClientsData has created" << std::endl;
}



std::string ClientManager::addClient(const std::string& firstname, const std::string& lastname, const std::string& email)
{
	std::string new_client_id = "";
	try 
	{
		pqxx::work tx{ this->connection };
		connection.prepare("prepared_insert_Client", "INSERT INTO Client(firstname, secondname) VALUES($1, $2);");//prepare prepared statement
		tx.exec_prepared("prepared_insert_Client", firstname, lastname);//prepared statement
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		pqxx::work tx2{ this->connection };
		new_client_id = tx2.query_value<std::string>("SELECT MAX(c.id) FROM Client c "
			"where firstname = \'" + firstname + "\' AND secondname = \'" + lastname + "\';"
		);

		connection.prepare("prepared_insert_ClientsData", "INSERT INTO ClientsData(email, phone_number, client_id) VALUES($1, $2, $3);");//prepare prepared statement
		tx2.exec_prepared("prepared_insert_ClientsData", email, "", new_client_id);//prepared statement
		tx2.commit();
		std::cout << "The client(" + firstname + ") has added";
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return new_client_id;
}

void ClientManager::addPhoneNumber(std::string clientId, const std::string& phoneNumber)
{
	try
	{
		pqxx::work tx{ this->connection };
		connection.prepare("prepared_update_ClientsData", "UPDATE ClientsData SET phone_number = $1 WHERE client_id = $2;");//prepare prepared statement
		//connection.prepare("prepared_new_ClientsData", "INSERT INTO ClientsData(email, phone_number, client_id) VALUES($1, $2, $3);");

		std::string exist_row_id = tx.query_value<std::string>("SELECT cd.id FROM ClientsData cd "//in case client_id is incorrect it throws an exception from here 
			"where client_id = " + clientId + ";"
		);

		if (exist_row_id != "")
		{
			tx.exec_prepared("prepared_update_ClientsData", phoneNumber, clientId);
		}
		else {
			std::cout << "The row with client_id = " + clientId + " is missing" << std::endl;
		}
		tx.commit();
		std::cout << "The row with client_id = " + clientId + " has updated" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "The row with client_id = " + clientId + " is missing" << std::endl;
		std::cout << e.what() << std::endl;
	}
}