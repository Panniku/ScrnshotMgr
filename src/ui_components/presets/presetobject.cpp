#include "presetobject.h"

PresetObject::PresetObject() {}

PresetObject::PresetObject(QString id, QString values)
{
    mPair.first = id;
    mPair.second = values;
}

PresetObject::PresetObject(QString id, int order, QString regionName, PresetType presetType, QRectF rect)
{
    mId = id;
    mOrder = order;
    mRegionName = regionName;
    mPresetType = presetType;
    mRect = rect;

    // i have no idea which ai generated this and im too lazy to understand it, sorry :(

    std::string x = std::to_string(mRect.x());
    std::string y = std::to_string(mRect.y());
    std::string w = std::to_string(mRect.width());
    std::string h = std::to_string(mRect.height());

    std::string r = x + ";" + y + ";" + w + ";" + h;
    std::vector<std::string> v = {
        mId.toStdString(),
        std::to_string(mOrder),
        mRegionName.toStdString(),
        std::to_string(mPresetType),
        r
    };

    std::string s = std::accumulate(std::begin(v), std::end(v), std::string(), [](std::string &ss, std::string &s)
    {
        return ss.empty() ? s : ss + ";" + s;
    });

    QString values = QString::fromStdString(s);
    qDebug() << values;
    mPair.first = id;
    mPair.second = values;
}

QString PresetObject::getId()
{
    return getValuePart(0);
}

int PresetObject::getOrder()
{
    QString s = getValuePart(1);
    return std::stoi(s.toStdString());
}

QString PresetObject::getRegionName()
{
    return getValuePart(2);
}

int PresetObject::getPresetIntType()
{
    QString s = getValuePart(3);
    return std::stoi(s.toStdString());
}

QRectF PresetObject::getRect()
{
    QString a = getValuePart(4);
    QString b = getValuePart(5);
    QString c = getValuePart(6);
    QString d = getValuePart(7);
    int x = std::stoi(a.toStdString());
    int y = std::stoi(b.toStdString());
    int w = std::stoi(c.toStdString());
    int h = std::stoi(d.toStdString());
    return QRectF(x, y, w, h);
}

PresetObject PresetObject::fromVariant(QVariant &variant)
{
    QStringList parts = variant.toString().split(';');
    if (parts.size() < 4) {
        return PresetObject();
    }
    QString key = parts[0];
    QString value = parts.mid(1).join(';');
    return PresetObject(key, value);
}

QVariant PresetObject::toVariant()
{
    return QVariant(mPair.first + ";" + mPair.second);
}

QString PresetObject::getValuePart(int index)
{
    QStringList parts = mPair.second.split(';');
    if (index >= 0 && index < parts.size()) {
        return parts.at(index);
    }
    return QString();
}
