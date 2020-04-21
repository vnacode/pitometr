#include "server.h"

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QFile>
#include <QHostAddress>
#include "settings.h"
#include <QJsonArray>
#include <QJsonDocument>

Server::Server(u_short nPort, QObject *parent) :
  QObject(parent),
  m_ptcpServer(new QTcpServer(this)),
  _port(nPort)
{
    started();
}

void Server::started()
{
    if (false == m_ptcpServer->listen(QHostAddress::Any, _port))
    {
        m_ptcpServer->close();
       // LOG_ERROR("Server ERROR");
        throw m_ptcpServer->errorString();
    }

    connect(m_ptcpServer, SIGNAL(newConnection()), SLOT(on_newConnection()));
    connect(this,SIGNAL(apkMessageRecieved(QVariantMap)),this,SLOT(processingCanData(QVariantMap)));
 //  LOG_INFO("Server started on port:%1", QString::number(_port));

  //  if (Vertical::Threads::addThread("APK_MESSAGE_PARSER",new QThread()))
  //  m_apkParser->moveToThread(Vertical::Threads::getThread("APK_MESSAGE_PARSER"));
//    else
//    {
//        LOG_ERROR("ERROR WITH moving APK_MESSAGE_PARSER to thread");
//    }
}

void Server::sendMessage(const QString &to, const QVariant &msg)
{
   const auto &iter = m_clients.find(to);
   if (iter != m_clients.end())
   {
       sendJSon(iter.value(),msg);
   }
}

void Server::on_newConnection()
{
    QTcpSocket* pclientSock = m_ptcpServer->nextPendingConnection();
   // qdebug("New connection: " << pclientSock->peerAddress().toString().remove("::ffff:"));
    QObject::connect(pclientSock, SIGNAL(disconnected()),this, SLOT(onDisconnected()));
    QObject::connect(pclientSock, &QTcpSocket::readyRead,this,&Server::on_readyread);

//    if (pclientSock->peerAddress().toString().remove("::ffff:") == Settings::get_main_setting("IP_CONFIG","APK") ||
//            pclientSock->peerAddress().toString().remove("::ffff:") == Settings::get_main_setting("STAND_IP_CONFIG","APK")
//        )
//    {

}




void Server::on_readyread()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if (!socket)
        return;
    QByteArray buffer;
    buffer = socket->readAll();

    if (socket == m_clients.value("APK"))
    {
      // m_apkParser->addMessageToParse(buffer);
    }
}


void Server::onDisconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    const QString key = m_clients.key(socket);
    m_clients.remove(key);

    socket->deleteLater();
}

void Server::processingCanData(const QVariantMap &message)
{
     if(message["message-type"].toString() == "PU1173")
     {
         sendMessage("MFI_1",message );
         sendMessage("MFI_2",message );
     }
    if(message["message-type"].toString() == "CAN")
    {
       if (message["Block"].toString() == "MFI_1_ID")
       {
           sendMessage("MFI_1",message );
       }
       if (message["Block"].toString() == "MFI_2_ID")
       {
           sendMessage("MFI_2",message);
       }
       parseCan(message);
    }
}



void Server::parseCan(const QVariantMap &message)
{

}


void Server::sendJSon(QTcpSocket *sock, const QVariant& msg)
{
    QJsonDocument doc = QJsonDocument::fromVariant(msg);
    if (doc.isEmpty())
    {
       // LOG_ERROR("Fail to Send message");
        return;
    }
    QByteArray bytes = doc.toJson();
        sock->write(bytes);

}
