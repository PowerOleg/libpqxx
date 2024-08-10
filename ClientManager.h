#pragma once
#include <pqxx/pqxx>
#include <Windows.h>

class ClientManager {
public:
    ClientManager(std::string login, std::string password);  //�����������, ������� ����������� ����������, ����� ���� � �����������
    void initDbStructure(); // �����, ��������� ��������� �� (�������)
    int addClient(pqxx::connection& conn, const std::string& firstName, const std::string& lastName, const std::string& email); //������ ������� � ���������� ��� clientId
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