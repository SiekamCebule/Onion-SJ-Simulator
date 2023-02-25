#include "CountryFlagsManager.h"

#include <QPixmap>
#include <QString>
#include <QDebug>
#include <QSize>

CountryFlagsManager::CountryFlagsManager()
{

}

QPixmap CountryFlagsManager::getFlagPixmap(const QString &twoLettersCountryCode)
{
    return QPixmap("flags/" + twoLettersCountryCode + ".svg");
}

QString CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(const QString &code)
{
    if(code == "afg") return "af";
    else if(code == "alb") return "al";
    else if(code == "dza") return "dz";
    else if(code == "asm") return "as";
    else if(code == "and") return "ad";
    else if(code == "ago") return "ao";
    else if(code == "aia") return "ai";
    else if(code == "ata") return "aq";
    else if(code == "atg") return "ag";
    else if(code == "arg") return "ar";
    else if(code == "arm") return "am";
    else if(code == "abw") return "aw";
    else if(code == "aus") return "au";
    else if(code == "aut") return "at";
    else if(code == "aze") return "az";
    else if(code == "bhs") return "bs";
    else if(code == "bhr") return "bh";
    else if(code == "bgd") return "bd";
    else if(code == "brb") return "bb";
    else if(code == "blr") return "by";
    else if(code == "bel") return "be";
    else if(code == "blz") return "bz";
    else if(code == "ben") return "bj";
    else if(code == "bmu") return "bm";
    else if(code == "btn") return "bt";
    else if(code == "bol") return "bo";
    else if(code == "bes") return "bq";
    else if(code == "bih") return "ba";
    else if(code == "bwa") return "bw";
    else if(code == "bvt") return "bv";
    else if(code == "bra") return "br";
    else if(code == "iot") return "io";
    else if(code == "brn") return "bn";
    else if(code == "bgr") return "bg";
    else if(code == "bfa") return "bf";
    else if(code == "bdi") return "bi";
    else if(code == "cpv") return "cv";
    else if(code == "khm") return "kh";
    else if(code == "cmr") return "cm";
    else if(code == "can") return "ca";
    else if(code == "cym") return "ky";
    else if(code == "caf") return "cf";
    else if(code == "tcd") return "td";
    else if(code == "chl") return "cl";
    else if(code == "chn") return "cn";
    else if(code == "cxr") return "cx";
    else if(code == "cck") return "cc";
    else if(code == "col") return "co";
    else if(code == "com") return "km";
    else if(code == "cod") return "cd";
    else if(code == "cog") return "cg";
    else if(code == "cok") return "ck";
    else if(code == "cri") return "cr";
    else if(code == "hrv") return "hr";
    else if(code == "cub") return "cu";
    else if(code == "cuw") return "cw";
    else if(code == "cyp") return "cy";
    else if(code == "cze") return "cz";
    else if(code == "civ") return "ci";
    else if(code == "dnk") return "dk";
    else if(code == "dji") return "dj";
    else if(code == "dma") return "dm";
    else if(code == "dom") return "do";
    else if(code == "ecu") return "ec";
    else if(code == "egy") return "eg";
    else if(code == "slv") return "sv";
    else if(code == "gnq") return "gq";
    else if(code == "eri") return "er";
    else if(code == "est") return "ee";
    else if(code == "swz") return "sz";
    else if(code == "eth") return "et";
    else if(code == "flk") return "fk";
    else if(code == "fro") return "fo";
    else if(code == "fji") return "fj";
    else if(code == "fin") return "fi";
    else if(code == "fra") return "fr";
    else if(code == "guf") return "gf";
    else if(code == "pyf") return "pf";
    else if(code == "atf") return "tf";
    else if(code == "gab") return "ga";
    else if(code == "gmb") return "gm";
    else if(code == "geo") return "ge";
    else if(code == "deu") return "de";
    else if(code == "gha") return "gh";
    else if(code == "gib") return "gi";
    else if(code == "grc") return "gr";
    else if(code == "grl") return "gl";
    else if(code == "grd") return "gd";
    else if(code == "glp") return "gp";
    else if(code == "gum") return "gu";
    else if(code == "gtm") return "gt";
    else if(code == "ggy") return "gg";
    else if(code == "gin") return "gn";
    else if(code == "gnb") return "gw";
    else if(code == "guy") return "gy";
    else if(code == "hti") return "ht";
    else if(code == "hmd") return "hm";
    else if(code == "vat") return "va";
    else if(code == "hnd") return "hn";
    else if(code == "hkg") return "hk";
    else if(code == "hun") return "hu";
    else if(code == "isl") return "is";
    else if(code == "ind") return "in";
    else if(code == "idn") return "id";
    else if(code == "irn") return "ir";
    else if(code == "irq") return "iq";
    else if(code == "irl") return "ie";
    else if(code == "imn") return "im";
    else if(code == "isr") return "il";
    else if(code == "ita") return "it";
    else if(code == "jam") return "jm";
    else if(code == "jpn") return "jp";
    else if(code == "jey") return "je";
    else if(code == "jor") return "jo";
    else if(code == "kaz") return "kz";
    else if(code == "ken") return "ke";
    else if(code == "kir") return "ki";
    else if(code == "prk") return "kp";
    else if(code == "kor") return "kr";
    else if(code == "kwt") return "kw";
    else if(code == "kgz") return "kg";
    else if(code == "lao") return "la";
    else if(code == "lva") return "lv";
    else if(code == "lbn") return "lb";
    else if(code == "lso") return "ls";
    else if(code == "lbr") return "lr";
    else if(code == "lby") return "ly";
    else if(code == "lie") return "li";
    else if(code == "ltu") return "lt";
    else if(code == "lux") return "lu";
    else if(code == "mac") return "mo";
    else if(code == "mdg") return "mg";
    else if(code == "mwi") return "mw";
    else if(code == "mys") return "my";
    else if(code == "mdv") return "mv";
    else if(code == "mli") return "ml";
    else if(code == "mlt") return "mt";
    else if(code == "mhl") return "mh";
    else if(code == "mtq") return "mq";
    else if(code == "mrt") return "mr";
    else if(code == "mus") return "mu";
    else if(code == "myt") return "yt";
    else if(code == "mex") return "mx";
    else if(code == "fsm") return "fm";
    else if(code == "mda") return "md";


    else return "xx";
}

QSize CountryFlagsManager::getFlagPixmapSize(short type)
{
    switch(type)
    {
    case Normal:
        return QSize(39, 27);
    }
}
