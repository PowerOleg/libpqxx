#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#include <tuple>
#pragma execution_character_set("utf-8")


int main(int argc, char** argv)
{
	//setlocale(LC_ALL, "ru");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "Program has started" << std::endl;
	try
	{
		pqxx::connection conn("host=localhost port=5432 dbname=cpp_integration user=postgres password=106601333454");
		std::cout << "Connection is successful" << std::endl;
		pqxx::work tx{ conn };

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



		//add additional phone_number
		/*tx.exec("INSERT INTO ClientsData(email, phone_number, client_id) "
			"VALUES('p111@yandex.ru', '118', 2);"
		);
		tx.commit();
		*/


		/*
		//code to update field
		tx.exec("UPDATE client SET secondname='Вали' where firstname='Солимр Ибн';");
		std::cout << "secondname has updated" << std::endl;

		tx.exec("UPDATE clientsdata SET phone_number='' where client_id=(select id from client c where c.secondname like ('Долго%')) and id=3;");
		tx.commit();
		std::cout << "the phone_number of \"Долгопрудов\" has updated " << std::endl;
		*/
	


		//code to delete client
		/*tx.exec("DELETE from Client where id=3;");
		tx.commit();
		std::cout << "A client has deleted";
		*/




		//select query
		//auto values = tx.query<std::string, std::string>("SELECT * FROM books");
	
		//for (std::tuple<std::string, std::string> value : values)
		//{
		//	std::cout << std::get<0>(value) << std::endl;
		//}
	
	
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}




	return 0;
}