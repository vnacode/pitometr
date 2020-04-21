#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QObject>
#include <QList>
#include <QVariant>
#include <QHostAddress>
#include <QTcpServer>
#include <QThread>
#include <QRunnable>
#include <settings.h>

typedef unsigned short u_short;

class Server : public QObject
{
    Q_OBJECT
public:
    Q_SIGNAL void apkMessageRecieved(const QVariantMap &msg);

    static Server* getInstance()
    {
        static Server *instance = new Server(Settings::get_main_setting("PORTS","RMI").toInt());
        return instance;
    }

    Server(u_short nPort = 2020, QObject *parent = nullptr);
public slots:
    void started();
    void sendMessage(const QString &to, const QVariant &msg);
    void on_readyread();
protected:

    QTcpServer* m_ptcpServer;
protected slots:
    void on_newConnection();


private slots:
    void parseCan(const QVariantMap &message);
    void onDisconnected();
    void processingCanData(const QVariantMap &message);
private:

    //  Server &operator=(Server server) = delete;
    u_short _port;
    QHash<QString ,QTcpSocket*> m_clients; //We need a buffer to store data until block has completely received
    inline void sendJSon(QTcpSocket *sock, const QVariant &msg);
};



#endif // SERVER_H
