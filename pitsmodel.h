#ifndef PITSMODEL_H
#define PITSMODEL_H

#include <QAbstractListModel>
#include <QTimer>
#include <QDir>

//#include <QSortFilterProxyModel>


class BaseTarget
{
public:
    BaseTarget() {}
    BaseTarget(int id, const QString &name, const QString &iconPath = "", int respawnTimeMin = 1200, int respawnTimeMax = 1200)
        : targetId(id),
          m_name(name),
          m_iconPath(iconPath),
          m_timeRemaning(respawnTimeMin),
          m_respawnTimeMin(respawnTimeMin),
          m_respawnTimeMax(respawnTimeMax)
    {}

    // seters
    void setName(const QString &name) { m_name = name; }
    void setIconPath(const QString &path) { m_iconPath = path; }
    void setTimeRemaning(int time) { m_timeRemaning = time; }

    // geters
    int getId() const { return  targetId; }
    QString getName() const { return m_name; }
    QString getIconPath() const { return m_iconPath; }
    int getTimeRemaning() const { return m_timeRemaning;}
    inline int getMinimumRespawnTime() const { return  m_respawnTimeMin; }
     inline int getMaximumRespawnTime() const { return  m_respawnTimeMax; }

    inline bool isEmpty() const
    {
        return m_name.isEmpty();
    }

    void tick()
    {
        m_timeRemaning--;
    }

    inline bool operator==(const BaseTarget &rhs) { return rhs.targetId == this->targetId; }
    inline bool operator<(const BaseTarget &rhs) { return rhs.m_timeRemaning < this->m_timeRemaning; }

private:
    int targetId;
    int m_timeRemaning;
    int m_respawnTimeMin;
    int m_respawnTimeMax;
    QString m_name;
    QString m_iconPath;


};


class PitsModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum TargetRoles {
        DisplayRole,
        TimeRemaingRole = Qt::UserRole + 1,
        PicNameRole
        // Todo country, lat, lon
    };

    explicit PitsModel(const QList<BaseTarget> &list, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setAllTargets(const QList<BaseTarget> &list) { m_AllTargets = list; }

    // Add data:
    //  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    //   bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void addTarget (const BaseTarget &target);
    Q_INVOKABLE void addTarget(const QString &targetName);
        Q_INVOKABLE void addTarget(const int id, const int remainingTime);
    Q_INVOKABLE void updateTimer(const QString &targetName);
    Q_INVOKABLE void removeTarget(const QString &targetName);
//    Q_INVOKABLE QSortFilterProxyModel* getProxy() { return proxy;}
    Q_INVOKABLE void saveModel(const QString &file = QDir::currentPath()+"/last.json");

    void loadModel(const QString &file = QDir::currentPath()+"/last.json");

Q_SIGNALS:
    void targetsCountChanged();
protected:
  //  QSortFilterProxyModel *proxy;
    QTimer *m_updateTimer;
    QList<BaseTarget> m_targets;
    QList<BaseTarget> m_AllTargets;
    QHash<int, QByteArray> roleNames() const;

private:
    Q_SLOT void checkTargetCounts();
    Q_SLOT void tick();


};












class ALLTargetModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CitylRoles {
        NameRole = Qt::UserRole + 1

    };

    ALLTargetModel(QObject *parent = 0);
    void addTarget(const int id, const QString &name, const QString &picPath, const int respawnTimeMin,const int respawnTimeMax);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QList<BaseTarget> &getAllTargets() { return  m_targets; }
protected:
    QList<BaseTarget> m_targets;
    QHash<int, QByteArray> roleNames() const;

};













#endif // PITSMODEL_H
