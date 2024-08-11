#include "ClientManager.h"
#include <iostream>

ClientManager::ClientManager(pqxx::connection conn) : connection{ std::move(conn) }
{
	//setlocale(LC_ALL, "ru");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "Program has started" << std::endl;
	try
	{

		//pqxx::connection conn("host=localhost port=5432 dbname=" + db + " user=" + login + " password=" + password);
		//this->connection = std::move(conn);
		std::cout << "Connection is successful" << std::endl;
	}
		catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
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

/*void insert_into(pqxx::connection& conn)
{
	pqxx::work tx{ conn };
	conn.prepare("prepared_insert", "INSERT INTO Client(firstname, secondname) VALUES($1, $2);");//study prepared statement

	tx.exec("INSERT INTO Client(firstname, secondname) "
		"VALUES('Солимр Ибн', 'Вали Барад'), "
		"('Петя', 'Долгопрудов'), "
		"('Алеша', 'Кузькин');"
	);

	tx.exec("INSERT INTO ClientsData(email, phone_number, client_id) "
		"VALUES('solimr@mail.ru', '123456', 1), "
		"('ibnvalibarad@yandex.ru', '123456', 1), "
		"('ppp555@yahoo.com', '+1(243)-254-21', 2);"
	);

	tx.exec("INSERT INTO ClientsData(email, phone_number, client_id) "
		"VALUES('p111@yandex.ru', '118', 2);"
	);

	tx.exec_prepared("prepared_insert", "Алеша2", "Кузькин2");//study prepared statement
	tx.commit();
	std::cout << "Tables were filled" << std::endl;
}*/


std::string ClientManager::addClient(const std::string& firstname, const std::string& lastname, const std::string& email)
{
	std::string client_id = "";
	try
	{
	pqxx::work tx{ this->connection };
	
	try 
	{
	connection.prepare("prepared_insert_Client", "INSERT INTO Client(firstname, secondname) VALUES($1, $2);");//prepare prepared statement
	tx.exec_prepared("prepared_insert", firstname, lastname);//prepared statement
	tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		client_id = tx.query_value<std::string>("SELECT c.id FROM Client c "
			"where firstname = \'" + firstname + "\' AND secondname = \'" + lastname + "\';"
		);

		connection.prepare("prepared_insert_ClientsData", "INSERT INTO ClientsData(id, email, phone_number, client_id) VALUES($1, $2, $3);");//prepare prepared statement
		tx.exec_prepared("prepared_insert_ClientsData", email, "", client_id);//prepared statement
		tx.commit();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}


	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return client_id;
}
