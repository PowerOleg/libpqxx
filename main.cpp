#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#include <tuple>
#include <set>
#pragma execution_character_set("utf-8")

void update_query(pqxx::connection& conn, std::string table, std::string column, std::string value, std::string where)
{
	//code to insert into
	pqxx::work tx{ conn };
	tx.exec("INSERT INTO "+table+"(firstname, secondname) "
		"VALUES('Солимр Ибн', 'Вали Барад'), "
		"('Петя', 'Долгопрудов'), "
		"('Алеша', 'Кузькин')"
		";");
	tx.exec("INSERT INTO "+table+"(email, phone_number, client_id) "
		"VALUES('solimr@mail.ru', '123456', 1), "
		"('ibnvalibarad@yandex.ru', '123456', 1), "
		"('ppp555@yahoo.com', '+1(243)-254-21', 2);"
		);

	tx.commit();
	std::cout << "tables were filled" << std::endl;
	
}

void update_query(pqxx::connection& conn, std::string table, std::string column, std::string value, std::string where)
{
	pqxx::work tx{ conn };
	tx.exec("UPDATE "+table+" SET "+column+"="+value+" where "+where+";");
	tx.commit();
	std::cout << "Column " << column << " has updated" << std::endl;
}

std::set<std::string> select_query(pqxx::connection& conn) 
{
	pqxx::work tx{ conn };
	auto values = tx.query<std::string, std::string>("SELECT c.firstname, c.secondname FROM Client c "
		"JOIN Clientsdata cd on cd.client_id = c.id "
		"where cd.phone_number like ('%1%');"
	);

	std::set<std::string> result_set;

	for (std::tuple<std::string, std::string> value : values)
	{
		result_set.insert(std::get<0>(value) + " " + std::get<1>(value));
		//std::cout << "Name: " << std::get<0>(value) << ", secondname: " << std::get<1>(value) << std::endl;
	}

	return result_set;
}

int main(int argc, char** argv)
{
	//setlocale(LC_ALL, "ru");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "Program has started" << std::endl;
	try
	{
		pqxx::connection conn("host=localhost port=5432 dbname=cpp_integration user=postgres password=10");
		std::cout << "Connection is successful" << std::endl;
		//pqxx::work tx{ conn };

		//code to ctreate tables
		/*
		tx.exec("CREATE TABLE IF NOT EXISTS Client ("
		"id SERIAL PRIMARY KEY, "
		"firstname varchar(255) NOT NULL, "
		"secondname varchar(255) NOT NULL);"
		);

		tx.exec("CREATE TABLE IF NOT EXISTS ClientsData ("
			"id SERIAL PRIMARY KEY, "
			"email varchar(255), "
			"phone_number varchar(255), "
			"client_id int references Client(id));"
		);

		tx.commit();
		std::cout << "table's created" << std::endl;
		*/



		//code to insert into
		/*tx.exec("INSERT INTO Client(firstname, secondname) "
			"VALUES('Солимр Ибн', 'Вали Барад'), "
			"('Петя', 'Долгопрудов'), "
			"('Алеша', 'Кузькин')"
			";");
		tx.exec("INSERT INTO ClientsData(email, phone_number, client_id) "
			"VALUES('solimr@mail.ru', '123456', 1), "
			"('ibnvalibarad@yandex.ru', '123456', 1), "
			"('ppp555@yahoo.com', '+1(243)-254-21', 2);"
			);

		tx.commit();
		std::cout << "tables were filled" << std::endl;
		*/
		insert_into();


		//add additional phone_number
		/*tx.exec("INSERT INTO ClientsData(email, phone_number, client_id) "
			"VALUES('p111@yandex.ru', '118', 2);"
		);
		tx.commit();
		*/

		//update
		//update_query(conn, "client", "secondname", "\'Вали\'", "firstname=\'Солимр Ибн\'");
		//update_query(conn, "clientsdata", "phone_number", "\'\'", "client_id=(select id from client c where c.secondname like (\'Долго%\')) and id=3");

		//code to delete client
		/*tx.exec("DELETE from Client where id=3;");
		tx.commit();
		std::cout << "A client has deleted";
		*/




		
		//select
		//std::set<std::string> result_set = select_query(conn);
		//for (std::string str : result_set)
		//{
		//	std::cout << "Name: " << str << std::endl;
		//}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}




	return 0;
}