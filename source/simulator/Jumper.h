#ifndef JUMPER_H
#define JUMPER_H

#include "../utilities/ClassWithID.h"
#include "JumperSkills.h"

#include <QString>
#include <QPixmap>
#include <QObject>
#include <QJsonObject>

class Jumper : public ClassWithID
{
public:
    Jumper(const QString &name = "", const QString &surname = "", const QString &countryCode = "", const JumperSkills & skills = JumperSkills());
    ~Jumper();

    QPixmap getImagePixmap();
    QString getTextInfo();
    QString getTextForDiscord();

private:
    QString name, surname;
    QString countryCode;
    QPixmap flagPixmap;
    QString imageName;
    double personalBest;

    JumperSkills jumperSkills;
public:
    static QJsonObject getJsonObject(const Jumper &jumper);
    static Jumper getFromJson(QJsonObject obj);
    static QVector<Jumper> getVectorFromJson(const QByteArray & bytes);
    static void setupJumpersFlagPixmaps(QVector<Jumper> & jumpers);

    QString getName() const;
    void setName(const QString &newName);
    QString getSurname() const;
    void setSurname(const QString &newSurname);
    QString getNameAndSurname() const;
    QString getCountryCode() const;
    void setCountryCode(const QString &newCountryCode);
    JumperSkills getJumperSkills() const;
    JumperSkills * getJumperSkillsPointer() const;
    void setJumperSkills(const JumperSkills &newJumperSkills);
    QPixmap getFlagPixmap() const;
    void setFlagPixmap(const QPixmap &newFlagPixmap);
    double getPersonalBest() const;
    void setPersonalBest(double newPersonalBest);
    QString getImageName() const;
    void setImageName(const QString &newImageName);

    void updateCountryFlagPixmap();
};

#endif // JUMPER_H
