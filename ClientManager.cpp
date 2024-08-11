#include "ClientManager.h"
#include <iostream>

ClientManager::ClientManager(pqxx::connection&& conn) : connection { std::move(conn) }
{
	this->connection.prepare("prepared_insert_Client", "INSERT INTO Client(firstname, secondname) VALUES($1, $2);");//prepare prepared statement
	this->connection.prepare("prepared_insert_ClientsData", "INSERT INTO ClientsData(email, phone_number, client_id) VALUES($1, $2, $3);");//prepare prepared statement
	this->connection.prepare("prepared_update_ClientsData", "UPDATE ClientsData SET phone_number = $1 WHERE client_id = $2;");//prepare prepared statement
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
	std::cout << "A table ClientsData has been created" << std::endl;
}

std::string ClientManager::addClient(const std::string& firstname, const std::string& lastname, const std::string& email)
{
	std::string new_client_id = "";
	try 
	{
		pqxx::work tx{ this->connection };
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

		
		tx2.exec_prepared("prepared_insert_ClientsData", email, "", new_client_id);//prepared statement
		tx2.commit();
		std::cout << "The client(" + firstname + ") has been added" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return new_client_id;
}

void ClientManager::setPhoneNumber(const std::string clientId, const std::string& phoneNumber)
{
	try
	{
		pqxx::work tx{ this->connection };
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
		std::cout << "The row with client_id = " + clientId + " has been updated" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "The row with client_id = " + clientId + " is missing" << std::endl;
		std::cout << e.what() << std::endl;
	}
}

void ClientManager::updateClient(const std::string id, const std::string& firstname, const std::string& lastname, const std::string& new_email)
{
	try
	{
		pqxx::work tx{ this->connection };
		tx.exec_params("UPDATE Client SET firstname = $1, secondname = $2 WHERE id = $3", firstname, lastname, id);
		tx.exec_params("UPDATE ClientsData SET email = $1 WHERE client_id = $2", new_email, id);

		tx.commit();
		std::cout << "The client with id = " + id + " has been updated" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void ClientManager::removeClient(const std::string clientId)
{
	try
	{
		pqxx::work tx{ this->connection };
		tx.exec("DELETE from clientsdata cd where cd.client_id = " + tx.esc(clientId));
		tx.exec("DELETE from Client where id = " + tx.esc(clientId));
		
		tx.commit();
		std::cout << "A client with id = " + clientId + " has been deleted" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
std::set<std::string> ClientManager::findClients(const std::string& searchValue)
{
	pqxx::work tx{ this->connection };
	auto values = tx.query<std::string, std::string>("SELECT c.firstname, c.secondname FROM Client c "
		"JOIN Clientsdata cd on cd.client_id = c.id "
		"where " + searchValue + ";"
	);

	std::set<std::string> result_set;
	for (std::tuple<std::string, std::string> value : values)
	{
		result_set.insert(std::get<0>(value) + " " + std::get<1>(value));
	}

	return result_set;
}

void ClientManager::print(std::set<std::string> values)
{
	for (std::string str : values)
	{
		std::cout << "Name: " << str << std::endl;
	}
}