#ifndef JUMPER_H
#define JUMPER_H

#include "../utilities/ClassWithID.h"
#include "JumperSkills.h"

#include <QString>


class Jumper : public ClassWithID
{
public:
    Jumper(const QString &name = "", const QString &surname = "", const QString &countryCode = "");

private:
    QString name, surname;
    QString countryCode;

    JumperSkills jumperSkills;

public:
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
};

#endif // JUMPER_H
