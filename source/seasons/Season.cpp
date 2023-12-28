#include "Season.h"

#include "QtConcurrent"

Season::Season() :
    Identifiable()
{
}

Season::~Season()
{
}

QJsonObject Season::getJsonObject(const Season &season)
{
    QJsonObject obj;
    obj.insert("id", season.getIDStr());
    obj.insert("season-number", season.getSeasonNumber());
    if(season.getActualCalendar() != nullptr)
        obj.insert("actual-calendar-id", season.getActualCalendar()->getIDStr());
    else
        obj.insert("actual-calendar-id", QString::number(-1));

    QFuture<QJsonObject> calendarsFuture = QtConcurrent::mapped(season.getCalendars(), [](SeasonCalendar * p){return SeasonCalendar::getJsonObject(*p);});
    QJsonArray calendarsArray;
    for(auto & o : calendarsFuture.results())
        calendarsArray.append(o);
    obj.insert("calendars", calendarsArray);

    return obj;
}

Season Season::getFromJson(QJsonObject obj, IdentifiableObjectsStorage * storage)
{
    Season season;
    season.setID(sole::rebuild(obj.value("id").toString().toStdString()));
    season.setSeasonNumber(obj.value("season-number").toInt());


    /*QJsonArray array = obj.value("calendars").toArray();
    QVector<QJsonValue> values;
    for(auto val : array)
        values.push_back(val);
    QFuture<SeasonCalendar *> calendarsFuture = QtConcurrent::mapped(values, [storage](const QJsonValue &value){
        return new SeasonCalendar(SeasonCalendar::getFromJson(value.toObject(), storage));
    });
    season.setCalendars(calendarsFuture.results().toVector());
    if(storage != nullptr)
        storage->add(&season.getCalendarsReference());*/

    QJsonArray array = obj.value("calendars").toArray();
    for(auto a : array)
    {
        season.getCalendarsReference().push_back(new SeasonCalendar(SeasonCalendar::getFromJson(a.toObject(), storage)));
    }
    if(storage != nullptr)
        storage->add(season.getCalendarsReference());

    if(obj.value("actual-calendar-id").toString() == -1)
        season.setActualCalendar(nullptr);
    else
        season.setActualCalendar(static_cast<SeasonCalendar *>(storage->get(obj.value("actual-calendar-id").toString())));

    return season;
}

bool Season::containsCalendarByName(QString n)
{
    for(auto & c : calendars)
    {
        if(c->getName() == n)
            return true;
    }
    return false;
}

SeasonCalendar *Season::getActualCalendar() const
{
    return actualCalendar;
}

void Season::setActualCalendar(SeasonCalendar *newActualCalendar)
{
    actualCalendar = newActualCalendar;
}

QVector<SeasonCalendar *> Season::getCalendars() const
{
    return calendars;
}

QVector<SeasonCalendar *> &Season::getCalendarsReference()
{
    return calendars;
}

void Season::setCalendars(const QVector<SeasonCalendar *> &newCalendars)
{
    calendars = newCalendars;
}

int Season::getSeasonNumber() const
{
    return seasonNumber;
}

void Season::setSeasonNumber(int newSeasonNumber)
{
    seasonNumber = newSeasonNumber;
}
