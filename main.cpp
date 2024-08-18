#include <iostream>
#include <pqxx/pqxx>
#include "ClientManager.h"
#include <set>
#pragma execution_character_set("utf-8")

int main(int argc, char** argv)
{
	//setlocale(LC_ALL, "ru");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
try
{
	ClientManager clientManager("host=localhost port=5432 dbname=cpp_integration user=postgres password=106");
	std::string new_client_id = clientManager.addClient("Солимр Ибн", "Вали Барад", "solimr@mail.ru");
	std::string new_client_id2 = clientManager.addClient("Солимр Ибн", "Вали Барад", "ibnvalibarad@yandex.ru");
	std::string new_client_id3 = clientManager.addClient("Петя", "Долгопрудов", "dolgopet@mail.ru");
	std::string new_client_id4 = clientManager.addClient("Алеша", "Кузькин", "ppp555@yahoo.com");
	std::string new_client_id5 = clientManager.addClient("Алеша", "Кузькин", "p111@yandex.ru");
	
	clientManager.setPhoneNumber(new_client_id, "123456");
	clientManager.setPhoneNumber("2", "777777");
	clientManager.setPhoneNumber(new_client_id4, "+1(243)-254-21");
	
	clientManager.updateClient("2", "Поросенок", "Пеппа", "pig@yandex.ru");
	
	clientManager.removeClient("1");

	std::set<std::string> result_set = clientManager.findClients("phone_number like ('%1%')");
	clientManager.print(result_set);
}
catch (const std::exception& e)
{
	std::cout << e.what() << std::endl;
}
	return 0;
}