#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#include <tuple>
#include <set>
#pragma execution_character_set("utf-8")

void create_table(pqxx::connection& conn, std::string table, std::string param)
{
	pqxx::work tx{ conn };
	tx.exec("CREATE TABLE IF NOT EXISTS " + table + " (" + param + ");");
	tx.commit();
	std::cout << "A table has created" << std::endl;
}

void insert_into(pqxx::connection& conn)
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
}

void delete_row_by_id(pqxx::connection& conn, std::string table, std::string id)
{
	pqxx::work tx{ conn };
	tx.exec("DELETE from " + table + " where id=" + id + ";");
	tx.commit();
	std::cout << "A client has deleted";

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
		pqxx::connection conn("host=localhost port=5432 dbname=cpp_integration user=postgres password=106");
		std::cout << "Connection is successful" << std::endl;

		//create+
		create_table(conn, 
			"Client",
			"id SERIAL PRIMARY KEY, "
			"firstname varchar(255) NOT NULL, "
			"secondname varchar(255) NOT NULL"
			);
		create_table(conn,
			"ClientsData",
			"id SERIAL PRIMARY KEY, "
			"email varchar(255), "
			"phone_number varchar(255), "
			"client_id int references Client(id)"
		);



		//insert+
		insert_into(conn);

		//update+
		update_query(conn, "client", "secondname", "\'Вали\'", "firstname=\'Солимр Ибн\'");
		update_query(conn, "clientsdata", "phone_number", "\'\'", "client_id=(select id from client c where c.secondname like (\'Долго%\')) and id=3");

		//delete row+
		delete_row_by_id(conn, "Client", "3");
		
		//select+
		std::set<std::string> result_set = select_query(conn);
		for (std::string str : result_set)
		{
			std::cout << "Name: " << str << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}