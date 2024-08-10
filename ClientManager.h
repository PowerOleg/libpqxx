#pragma once
#include <pqxx/pqxx>
#include <Windows.h>

class ClientManager {
public:
    ClientManager(std::string login, std::string password);  //конструктор, который настраивает соединение, может быть с параметрами
    void initDbStructure(); // Метод, создающий структуру БД (таблицы)
    int addClient(pqxx::connection& conn, const std::string& firstName, const std::string& lastName, const std::string& email); //создаём клиента и возвращаем его clientId
    void addPhoneNumber(int clientId, const std::string& phoneNumber);
    void updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email);
    void removeClient(int clientId);
    void findClient(const std::string& searchValue);

    void create_table(pqxx::connection& conn, std::string table, std::string param)
    {
        pqxx::work tx{ conn };
        tx.exec("CREATE TABLE IF NOT EXISTS " + table + " (" + param + ");");
        tx.commit();
    }
private:
    pqxx::connection connection;
};