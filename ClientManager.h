#pragma once
#include<iostream>
#include<pqxx/pqxx>
#include<Windows.h>
#include<set>
#pragma execution_character_set("utf-8")

class ClientManager {
public:
    ClientManager(const std::string& connectionString);  //�����������, ������� ����������� ����������, ����� ���� � �����������
    void initDbStructure(); // �����, ��������� ��������� �� (�������)
    std::string addClient(const std::string& firstname, const std::string& lastname, const std::string& email); //������ ������� � ���������� ��� clientId
    void setPhoneNumber(const std::string clientId, const std::string& phoneNumber);
    void updateClient(const std::string clientId, const std::string& firstname, const std::string& lastname, const std::string& email);
    void removeClient(const std::string clientId);
    std::set<std::string> findClients(const std::string& searchValue);
    void print(std::set<std::string> values);
    
private:
    pqxx::connection connection;
    void create_table(std::string table, std::string param);
};