#include "pitsmodel.h"
#include <QFile>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>



PitsModel::PitsModel(const QList<BaseTarget> &list, QObject *parent)
    : QAbstractListModel(parent)
{
    m_updateTimer = new QTimer();
    m_updateTimer->setInterval(1000);
    setAllTargets(list);
    connect(this,SIGNAL(targetsCountChanged()),this,SLOT(checkTargetCounts()));
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(tick()));

    loadModel();
}


int PitsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_targets.count();
}

void PitsModel::sort(int column, Qt::SortOrder order)
{
    if (column > 0)
        return;
    qDebug() << "Im here";
    std::sort(m_targets.begin(),m_targets.end(),[=](const BaseTarget &a, BaseTarget &b) -> bool
    {
        if (order == Qt::AscendingOrder)
            return (a.getTimeRemaning() < b.getTimeRemaning());
        else
            return (a.getTimeRemaning() > b.getTimeRemaning());
    });

    for (int i = 0; i < this->rowCount(); i++)
    emit dataChanged(index(i),index(i));

}

QVariant PitsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == DisplayRole)
        return m_targets[index.row()].getName();
    if (role == TimeRemaingRole)
        return m_targets[index.row()].getTimeRemaning();
    if (role == PicNameRole)
        return m_targets[index.row()].getIconPath();
    // FIXME: Implement me!
    return QVariant();
}

bool PitsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        switch (role) {
        case DisplayRole:
            m_targets[index.row()].setName(value.toString());
            break;
        default:
            break;
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags PitsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

//bool PitsModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endInsertRows();
//}

//bool PitsModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endRemoveRows();
//}

void PitsModel::addTarget(const BaseTarget &target)
{
    if (target.isEmpty())
        return;
    for (auto &t : m_targets)
    {
        if (t == target)
            return;
    }
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_targets << target;
    endInsertRows();
    emit targetsCountChanged();
}


void PitsModel::addTarget(const QString &name)
{
    for (const auto &t : m_AllTargets)
    {
        if (t.getName() == name)
        addTarget(t);
    }
}

void PitsModel::addTarget(const int id, const int remainingTime)
{
    for (const auto &t : m_AllTargets)
    {
        if (t.getId() == id)
        {
            BaseTarget bt = t;
            bt.setTimeRemaning(remainingTime);
            addTarget(bt);
        }
    }
}

void PitsModel::updateTimer(const QString &targetName)
{
    for (auto &t : m_targets)
    {
        if (t.getName() == targetName)
        t.setTimeRemaning(t.getMinimumRespawnTime());
    }
        this->sort(0);
}

void PitsModel::removeTarget(const QString &targetName)
{
    for(int i = 0; i < this->rowCount(); i++){
      QModelIndex index = this->index(i,0);

      if (m_targets[index.row()].getName() == targetName)
      {
       beginRemoveRows(QModelIndex(),index.row(),index.row());
       m_targets.removeOne(m_targets[index.row()]);
       endRemoveRows();
      }
    }
}

void PitsModel::saveModel(const QString &file)
{
    if (m_targets.count() == 0)
        return;

    QFile f(file);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QJsonObject obj;
    obj.insert("timeStamp", QJsonValue::fromVariant(QDateTime::currentSecsSinceEpoch()));
    QJsonArray arr;


    for (const auto &t : m_targets)
    {
        QJsonObject o;
        o.insert("id",QJsonValue::fromVariant(t.getId()));
        o.insert("name",QJsonValue::fromVariant(t.getName()));
        o.insert("remainingTime",QJsonValue::fromVariant(t.getTimeRemaning()));
        arr.push_back(o);
    }
    obj.insert("pits",QJsonValue::fromVariant(arr));
    QJsonDocument document(obj);
    if (document.isEmpty())
        return;
    f.write(document.toJson());
    qDebug() << f.fileName();
    f.close();
}

void PitsModel::loadModel(const QString &file)
{
    QFile _file(file);
    _file.open(QIODevice::ReadOnly | QIODevice::Text);
    const QString json = _file.readAll();
    _file.close();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    if (document.isNull())
        return;
    QJsonObject obj = document.object();
    const qint64 loadTimeStamp = obj["timeStamp"].toInt();
    const qint64 curtime = QDateTime::currentSecsSinceEpoch();
    if ((curtime - loadTimeStamp) > (60 * 60 * 12))
        return;
    QJsonArray ja =obj["pits"].toArray();
    for (const QJsonValue &value : ja)
    {
        QJsonObject obj = value.toObject();
        qint64 currentRemaningTime = curtime - loadTimeStamp - obj["remainingTime"].toInt();
         qDebug() << obj["name"].toString() << qAbs(currentRemaningTime);
        if (currentRemaningTime < 0)
        this->addTarget(obj["id"].toInt(),qAbs(currentRemaningTime));
    }
}


QHash<int, QByteArray> PitsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayRole] = "targetName";
    roles[TimeRemaingRole] = "timeRemaning";
    roles[PicNameRole] = "picName";
    return roles;
}

void PitsModel::checkTargetCounts()
{
    if (m_targets.count() > 0 && !m_updateTimer->isActive())
        m_updateTimer->start();
    if (m_targets.count() == 0 && m_updateTimer->isActive())
        m_updateTimer->stop();
}

void PitsModel::tick()
{

    for(int i = 0; i < this->rowCount(); i++){
      QModelIndex index = this->index(i,0);
      m_targets[index.row()].tick();
       emit dataChanged(index, index, QVector<int>() << TimeRemaingRole);
    }

}




///////////////////////////


ALLTargetModel::ALLTargetModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QFile file(":/Data/info.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    const QString json = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonArray ja = document.array();
    for (const QJsonValue &value : ja)
    {
        QJsonObject obj = value.toObject();
        this->addTarget(obj["id"].toInt(),obj["name"].toString(),obj["picName"].toString(),obj["respawnTimeMin"].toInt(),obj["respawnTimeMax"].toInt());
    }

    std::sort(m_targets.begin(),m_targets.end(),[=](const BaseTarget &a, BaseTarget &b) -> bool
    {
          return (a.getName() < b.getName());
    });
}

void ALLTargetModel::addTarget(const int id, const QString &name, const QString &picPath, const int respawnTimeMin, const int respawnTimeMax)
{
    BaseTarget t = BaseTarget(id,name,picPath,respawnTimeMin,respawnTimeMax);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_targets.append(t);
    endInsertRows();
}

int ALLTargetModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_targets.count();
}

QVariant ALLTargetModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_targets.count())
        return QVariant();

    if (role == NameRole)
        return m_targets[index.row()].getName();

    return QVariant();

}


QHash<int, QByteArray> ALLTargetModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}















