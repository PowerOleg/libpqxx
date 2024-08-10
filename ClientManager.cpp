#include "ClientManager.h"
#include <iostream>

ClientManager::ClientManager(std::string login, std::string password)
{
	//setlocale(LC_ALL, "ru");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "Program has started" << std::endl;
	try
	{
		pqxx::connection conn("host=localhost port=5432 dbname=cpp_integration user=postgres password=106");
		this->connection = std::move(conn);
		std::cout << "Connection is successful" << std::endl;
	}
		catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void ClientManager::create_table(pqxx::connection& connection, std::string table, std::string param)
{
	try
	{
	pqxx::work tx{ connection };
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
	create_table(connection,
		"Client",
		"id SERIAL PRIMARY KEY, "
		"firstname varchar(255) NOT NULL, "
		"secondname varchar(255) NOT NULL"
	);
	std::cout << "A table Client has created" << std::endl;
	create_table(connection,
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


int ClientManager::addClient(pqxx::connection& connection, const std::string& firstName, const std::string& lastName, const std::string& email)
{
	try 
	{
	pqxx::work tx{ connection };
	connection.prepare("prepared_insert", "INSERT INTO Client(firstname, secondname) VALUES($1, $2);");//study prepared statement

	tx.exec("INSERT INTO Client(firstname, secondname) "
		"VALUES('Солимр Ибн', 'Вали Барад'), "
		"('Петя', 'Долгопрудов'), "
		"('Алеша', 'Кузькин');"
	);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
