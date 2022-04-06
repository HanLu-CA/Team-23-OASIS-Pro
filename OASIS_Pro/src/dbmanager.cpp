#include "dbmanager.h"

//const QString DBManager::DATABASE_PATH = "/database/oasis.db";


//DBManager::DBManager(){
 //   oasisDB = QSqlDatabase::addDatabase("QSQLITE");
//    oasisDB.setDabaseName("oasis.db");

//    if (!denasDB.open()) throw "Error: Database could not be opened";

//    if (!DBInit()) throw "Error: Database could not be initialized";

//}

//bool DBManager::DBInit(){
//    oasisDB.transaction();

//    QSqlQuery query;
//    query.exec("CREATE TABLE IF NOT EXISTS frequencies (name TEXT NOT NULL);");
 //   query.exec("CREATE TABLE IF NOT EXISTS therapies (name TEXT NOT NULL);");
//    query.exec("CREATE TABLE IF NOT EXISTS profiles (pid INTEGER NOT NULL);");

 //   query.exec("INSERT OR REPLACE INTO frequencies VALUES ('3Hz'),('5Hz'),('8Hz'),('11Hz'),('15Hz'),('22Hz'),('100Hz');");
 //   query.exec("INSERT OR REPLACE INTO therapies VALUES ('MET','3Hz'),('Sub-Delta','3Hz'),('Delta','5Hz'),('Theta','8Hz'),('Alpha','11Hz'),('Beta 1','15Hz'),('Beta 2','22Hz'),('Beta 3','100Hz');");

 //   return oasisDB.commit();


//}

//Profile* DBManager::getProfile(int id) {
 //   oasisDB.transaction();

  //  QSqlQuery query;

//}
