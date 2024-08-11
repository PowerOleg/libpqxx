#pragma once
#include<iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#pragma execution_character_set("utf-8")

class ClientManager {
public:
    ClientManager(pqxx::connection&& conn);  //конструктор, который настраивает соединение, может быть с параметрами
    void initDbStructure(); // Метод, создающий структуру БД (таблицы)
   std::string addClient(const std::string& firstname, const std::string& lastname, const std::string& email); //создаём клиента и возвращаем его clientId
    void setPhoneNumber(const std::string clientId, const std::string& phoneNumber);
    void updateClient(const std::string clientId, const std::string& firstname, const std::string& lastname, const std::string& email);
    void removeClient(const std::string clientId);
    void findClient(const std::string& searchValue);

    void create_table(std::string table, std::string param);
    /* {
        pqxx::work tx{ conn };
        tx.exec("CREATE TABLE IF NOT EXISTS " + table + " (" + param + ");");
        tx.commit();
    }*/
private:
    pqxx::connection connection;
};