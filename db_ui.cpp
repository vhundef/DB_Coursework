//
// Created by Vladimir on 31.05.2021.
//

#include "db_ui.hpp"
#include "Common.hpp"
#include <QGuiApplication>
#include <QScreen>
#include <QTableWidget>
#include <spdlog/spdlog.h>
DbUI::DbUI(DatabaseManager *databaseManager)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    resize(width / 2, height / 2);
    if (databaseManager == nullptr) {
        spdlog::critical("DbUI::databaseManager is null");
    }
    dbManager = databaseManager;

    DbUI::setupConnections();
    initializeElements();
    setupUI();
}
void DbUI::initializeElements()
{
    setLayout(&gridLayout);
    // initializing buttons
    back_btn.setText("<-");
    add_btn.setText("Добавить");
    edit_btn.setText("Редактировать");
    delete_btn.setText("Удалить");
}

void DbUI::setupUI()
{
    windowHeader.setText("ВОЕМНХ");
    table_widget.setColumnCount(2);
    table_widget.setHorizontalHeaderLabels({"Server", "Status"});
    table_widget.setSelectionBehavior(QAbstractItemView::SelectRows);
    table_widget.setSelectionMode(QAbstractItemView::SingleSelection);
    table_widget.verticalHeader()->hide();

    windowHeader.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    add_btn.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    edit_btn.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    delete_btn.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    table_widget.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridLayout.addWidget(&windowHeader, 0, 0, 1, 2);
    gridLayout.addWidget(&back_btn, 1, 0);
    gridLayout.addWidget(&table_widget, 2, 0, 3, 1);
    gridLayout.addWidget(&add_btn, 2, 1);
    gridLayout.addWidget(&edit_btn, 3, 1);
    gridLayout.addWidget(&delete_btn, 4, 1);

    // Font size related manipulations
    auto font = windowHeader.font();
    font.setPixelSize(20);
    windowHeader.setFont(font);
}
void DbUI::setupConnections()
{
    spdlog::info("DB_UI::setupConnections");
    connect(&back_btn, &QPushButton::clicked, this, [this]() {
        spdlog::info("Layout switch: Whatever->mainMenu");
        close();
    });
    //   connect(dbManager,&DatabaseManager::queryResultReady,this,&DbUI::processQuery);
    connect(dbManager->worker(), &DatabaseWorker::queryResultReady, this, &DbUI::processQuery);
}
void DbUI::setTableHeaders(const QList<QString> &tableHeaders)
{
    table_widget.setColumnCount(tableHeaders.size());
    table_widget.setHorizontalHeaderLabels(tableHeaders);
}

void DbUI::setQueryString(const QString &query)
{
    queryString = query;
}

void DbUI::setWindowHeaderAndTitle(QString windowHeader_)
{
    windowHeader.setText(windowHeader_);
    setWindowTitle(windowHeader_);
}

void DbUI::loadDataFromDb()
{
    dbManager->loadData(queryString, dbManager->connection());
}
void DbUI::processQuery(QList<QString> results)
{
    spdlog::info("DB_UI::processQuery {");
    for (auto &res : results) {
        spdlog::info("  {}", res.toStdString());
    }
    spdlog::info("}");
    table_widget.insertRow(table_widget.rowCount());
    for (int i = 0; i < results.size(); ++i) {
        table_widget.setItem(table_widget.rowCount() - 1, i, new QTableWidgetItem(results.at(i)));
    }
}
// ContractsUI Class
void ContractsUI::setupConnections()
{
    spdlog::info("ContractsUI::setupConnections");
    DbUI::setupConnections();
}
ContractsUI::ContractsUI(DatabaseManager *databaseManager) : DbUI(databaseManager)
{
    setTableHeaders({"ID", "Дата заключения", "Поставщик", "Покупатель", "Товар", "Цена 1шт", "Кол-во", "Сумма"});
    setQueryString("select * from public.\"Contract\"");
    loadDataFromDb();
}

// clients UI

SuppliersUI::SuppliersUI(DatabaseManager *databaseManager) : DbUI(databaseManager)
{
    setTableHeaders({"Тут", "нада", "что-то написать"});
}
void SuppliersUI::setupConnections()
{
    DbUI::setupConnections();
}
