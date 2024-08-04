#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#include <tuple>
#pragma execution_character_set("utf-8")


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "Program has started" << std::endl;
	try
	{
		pqxx::connection conn("host=localhost port=5432 dbname=cpp_integration user=postgres password=10");
		std::cout << "Connection is successful";

		pqxx::work tx{ conn };
		auto values = tx.query<std::string, std::string>("SELECT * FROM books");
	
		for (std::tuple<std::string, std::string> value : values)
		{
			std::cout << std::get<0>(value) << std::endl;
		}
	
	
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}




	return 0;
}