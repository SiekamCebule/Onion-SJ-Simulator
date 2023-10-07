#include "WindGenerationSettings.h"
#include "../../global/MyRandom.h"
#include "../../utilities/functions.h"

WindGenerationSettings::WindGenerationSettings(short baseDirection, double windDirectionInstability, double baseWindStrength, double windStrengthInstability) : baseDirection(baseDirection),
    windDirectionInstability(windDirectionInstability),
    baseWindStrength(baseWindStrength),
    windStrengthInstability(windStrengthInstability)
{}

void WindGenerationSettings::randomizeSettings(QVector<WindGenerationSettings> *settings, double lotability, double prefferedDirection)
{
    if(prefferedDirection == (-1))
        prefferedDirection = MyRandom::randomDouble(0, 360);
    if(lotability == (-1))
    {
        lotability = MyRandom::normalDistributionRandom(5, 1.6);
        if(lotability < 0)
            lotability = 0;
        else if(lotability > 10)
            lotability = 10;
    }
    double baseWindStrength = MyRandom::normalDistributionRandom(1 + lotability / 8.5, lotability / 9);
    for(auto & sett : *settings)
    {
        double windDirection = prefferedDirection + MyRandom::normalDistributionRandom(0, lotability * 4);
        if(windDirection < 0)
        {
            windDirection = 360 - (-windDirection);
        }
        if(windDirection > 360)
        {
            windDirection -= (int(windDirection / 360)) * 360;
        }
        sett.setBaseDirection(windDirection);
        double windDirectionInstability = lotability * 27 + MyRandom::normalDistributionRandom(0, lotability * 4);
        sett.setWindDirectionInstability(windDirectionInstability);

        double windStrength = baseWindStrength + MyRandom::normalDistributionRandom(0, lotability / 11);
        sett.setBaseWindStrength(windStrength);
        double windStrengthInstability = lotability / 4 + MyRandom::normalDistributionRandom(0, lotability / 7);
        sett.setWindStrengthInstability(windStrengthInstability);
    }
}

double WindGenerationSettings::getBaseDirection() const
{
    return baseDirection;
}

void WindGenerationSettings::setBaseDirection(double newBaseDirection)
{
    baseDirection = newBaseDirection;
}

double WindGenerationSettings::getWindStrengthInstability() const
{
    return windStrengthInstability;
}

void WindGenerationSettings::setWindStrengthInstability(double newWindStrengthInstability)
{
    windStrengthInstability = newWindStrengthInstability;
}

double WindGenerationSettings::getBaseWindStrength() const
{
    return baseWindStrength;
}

void WindGenerationSettings::setBaseWindStrength(double newBaseWindStrength)
{
    baseWindStrength = newBaseWindStrength;
}

double WindGenerationSettings::getWindDirectionInstability() const
{
    return windDirectionInstability;
}

void WindGenerationSettings::setWindDirectionInstability(double newWindDirectionInstability)
{
    windDirectionInstability = newWindDirectionInstability;
}
