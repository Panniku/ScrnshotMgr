#ifndef PRESETOBJECT_H
#define PRESETOBJECT_H

#include <QString>
#include <QRectF>
#include <QVariant>

enum PresetType {
    Fullscreen = 0,
    Freedraw = 1,
    Custom = 2
};

class PresetObject
{
public:
    PresetObject();
    PresetObject(QString id, QString values);
    PresetObject(QString id, int order, QString regionName, PresetType presetType, QRectF rect);

    QString getId();
    int getOrder();
    QString getRegionName();
    int getPresetIntType();
    QRectF getRect();
    //

    //
    static PresetObject fromVariant(QVariant& variant);
    QVariant toVariant();
    QString getValuePart(int index);

private:
    QPair<QString, QString> mPair;
    QVariant mVariant;
    //
    QString mId;
    int mOrder;
    QString mRegionName;
    PresetType mPresetType;
    QRectF mRect;
    //

};

#endif // PRESETOBJECT_H
