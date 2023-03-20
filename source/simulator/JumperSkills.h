#ifndef JUMPERSKILLS_H
#define JUMPERSKILLS_H

#include "../utilities/ClassWithID.h"
#include "../utilities/ClassWithCharacteristics.h"
#include "Characteristic.h"

class Jumper;

#include <QSet>
#include <QStringList>

class JumperSkills : public ClassWithID, public ClassWithCharacteristics
{
public:
    JumperSkills(double takeoffPower = 0, double takeoffTechnique = 0, double flightTechnique = 0, short flightStyle = 0, double form = 0, double landingStyle = 0, const QSet<Characteristic> &characteristics = QSet<Characteristic>(), Jumper * jumper = nullptr);

private:
    double takeoffPower; // 1-50
    double takeoffTechnique; // 1-50
    short flightStyle; // 0-klasyczny, 1-Styl V, 2-Nowoczesne V, 3-Szerokie V, 4-Styl H
    double flightTechnique; // 1-50
    double form; // 1-50
    short jumpsEquality; // 1- bardzo nierówne,   2-  Nierówne,    3- Trochę nierówne,   4- Średnio równe,     5- Trochę bardziej równe,   6- Równe -    7- równe
    double landingStyle; // 1-15

    Jumper * jumper;
    ulong jumperID;

public:
    enum FlightStyle{
        VStyle,
        ModernVStyle,
        WideVStyle,
        HStyle
    };

    void checkSkillsLimits();

    static QString getJumpsEqualityString(short jumpsEquality);
    static QStringList getJumpsEqualityStringList();
public:
    void setJumperID(ulong ID);
    double getTakeoffPower() const;
    void setTakeoffPower(double newTakeoffPower);
    double getTakeoffTechnique() const;
    void setTakeoffTechnique(double newTakeoffTechnique);
    short getFlightStyle() const;
    void setFlightStyle(short newFlightStyle);
    double getFlightTechnique() const;
    void setFlightTechnique(double newFlightTechnique);
    double getLandingStyle() const;
    void setLandingStyle(double newLandingStyle);
    QSet<Characteristic> getCharacteristics() const;
    void setCharacteristics(const QSet<Characteristic> &newCharacteristics);
    double getForm() const;
    void setForm(double newForm);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    short getJumpsEquality() const;
    void setJumpsEquality(short newJumpsEquality);
};

#endif // JUMPERSKILLS_H
