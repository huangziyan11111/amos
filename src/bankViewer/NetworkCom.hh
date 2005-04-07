#ifndef NETWORKCOM_HH_
#define NETWORKCOM_HH_ 1

#include <qsocket.h>
#include <qserversocket.h>
#include <qapplication.h>
#include <qvbox.h>
#include <qtextview.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextstream.h>

#include <stdlib.h>


/*
  The ClientSocket class provides a socket that is connected with a client.
  For every client that connects to the server, the server creates a new
  instance of this class.
*/
class ClientSocket : public QSocket
{
    Q_OBJECT
public:
    ClientSocket( int sock, QObject *parent=0, const char *name=0 );
    ~ClientSocket();

signals:
    void logText( const QString& );
    void setContigLocation(QString, int);

private slots:
    void readClient();

private:
    int line;
};


/*
  The SimpleServer class handles new connections to the server. For every
  client that connects, it creates a new ClientSocket -- that instance is now
  responsible for the communication with that client.
*/
class SimpleServer : public QServerSocket
{
    Q_OBJECT
public:
    SimpleServer( QObject* parent=0 , int port=4242);
    ~SimpleServer();
    void newConnection( int socket );

signals:
    void newConnect( ClientSocket* );
};


/*
  The ServerInfo class provides a small GUI for the server. It also creates the
  SimpleServer and as a result the server.
*/
class ServerInfo : public QVBox
{
    Q_OBJECT
public:
    ServerInfo();
    ~ServerInfo();

private slots:
    void newConnect( ClientSocket *s );
    void connectionClosed();

private:
    QTextView *infoText;
};


#endif
