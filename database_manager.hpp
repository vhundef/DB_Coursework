//
// Created by Vladimir Shubarin on 27.05.2021.
//

#ifndef DB_COURSEWORK__DATABASE_MANAGER_HPP_
#define DB_COURSEWORK__DATABASE_MANAGER_HPP_

#include "Common.hpp"
#include "database_worker.hpp"
#include <QFuture>
#include <QList>
#include <QThread>
#include <pqxx/connection>
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    [[nodiscard]] QList<Server> &servers();
    [[nodiscard]] const pqxx::connection *connection();
    pqxx::connection *connectToServer(Server &server, bool use = true);
    void updateServerList();
    DatabaseManager();
    void addServer(Server &server);
    void closeConnection();
    ~DatabaseManager() override;

private:
    QList<Server> serverList;

    pqxx::connection *dbConnection{nullptr};
    QThread workerThread;
    static void loadServers(QList<Server> &serverListToFill);
signals:
    void isServerAvailable(Server *server);
    void serverAvailabilityResult(Server *server, bool);
};

#endif // DB_COURSEWORK__DATABASE_MANAGER_HPP_
