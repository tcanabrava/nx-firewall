//
// Created by alexis on 5/10/18.
//

#include <QMetaEnum>
#include <QVariantList>
#include "QVariantRuleSetConverter.h"

QVariantMap QVariantRuleSetConverter::toVariant(const RuleSet& ruleSet)
{
    QVariantMap res;
    return res;
}
RuleSet QVariantRuleSetConverter::toRuleSet(const QVariantMap& variantMap)
{
    return RuleSet();
}
QVariantMap QVariantRuleSetConverter::toVariant(const Rule& rule)
{
    QVariantMap res;

    auto actionEnum = QMetaEnum::fromType<Rule::Action>();
    auto directionEnum = QMetaEnum::fromType<Rule::Direction>();

    res[KEY_DIRECTION] = directionEnum.valueToKey(rule.getDirection());
    res[KEY_PROTOCOL] = rule.getProtocol();

    if (!rule.getSourceAddr().isEmpty())
        res[KEY_SOURCE_ADDR] = rule.getSourceAddr();

    if (!rule.getSourcePorts().isEmpty())
        res[KEY_SOURCE_PORTS] = getPortsToVariant(rule.getSourcePorts());

    if (!rule.getDestinationAddr().isEmpty())
        res[KEY_DESTINATION_ADDR] = rule.getDestinationAddr();

    if (!rule.getDestinationPorts().isEmpty())
        res[KEY_DESTINATION_PORTS] = getPortsToVariant(rule.getDestinationPorts());

    res[KEY_ACTION] = actionEnum.valueToKey(rule.getAction());

    return res;
}
QVariantList QVariantRuleSetConverter::getPortsToVariant(const QList<int>& portsList)
{
    QVariantList portsVariant;
    for (int port : portsList)
        portsVariant << port;
    return portsVariant;
}
Rule QVariantRuleSetConverter::toRule(QVariantMap map)
{
    Rule r;

    auto direction = getRuleDirection(map);
    r.setDirection(direction);

    auto action = getRuleAction(map);
    r.setAction(action);

    if (map.contains(KEY_SOURCE_ADDR)) {
        auto sourceAddr = map.value(KEY_SOURCE_ADDR).toString();
        r.setSourceAddr(sourceAddr);
    }


    if (map.contains(KEY_SOURCE_PORTS)) {
        auto variantList = map.value(KEY_SOURCE_PORTS).toList();
        auto sourcePorts = getPortsList(variantList);
        r.setSourcePorts(sourcePorts);
    }

    if (map.contains(KEY_DESTINATION_ADDR)) {
        auto destinationAddr = map.value(KEY_DESTINATION_ADDR).toString();
        r.setDestinationAddr(destinationAddr);
    }

    if (map.contains(KEY_DESTINATION_PORTS)) {
        auto variantList = map.value(KEY_DESTINATION_PORTS).toList();
        auto destinationPorts = getPortsList(variantList);
        r.setDestinationPorts(destinationPorts);
    }

    if (map.contains(KEY_PROTOCOL)) {
        auto protocol = map.value(KEY_PROTOCOL).toString();
        r.setProtocol(protocol);
    }


    return r;
}
QList<int> QVariantRuleSetConverter::getPortsList(const QList<QVariant>& variantList)
{
    QList<int> sourcePorts;
    for (const QVariant& v: variantList)
        sourcePorts << v.toInt();
    return sourcePorts;
}
Rule::Action QVariantRuleSetConverter::getRuleAction(const QVariantMap& map)
{
    if (!map.contains(KEY_ACTION))
        throw ConversionException{};

    auto actionEnum = QMetaEnum::fromType<Rule::Action>();
    auto actionValue = map.value(KEY_ACTION).toString().toStdString().c_str();
    bool succeeded = true;
    Rule::Action action = static_cast<Rule::Action>(actionEnum.keyToValue(actionValue, &succeeded));

    if (!succeeded)
        throw ConversionException{};

    return action;
}

Rule::Direction QVariantRuleSetConverter::getRuleDirection(const QVariantMap& map)
{
    if (!map.contains(KEY_DIRECTION))
        throw ConversionException{};

    auto directionEnum = QMetaEnum::fromType<Rule::Direction>();
    auto directionValue = map.value(KEY_DIRECTION).toString().toStdString().c_str();
    bool succeeded = true;
    Rule::Direction direction = static_cast<Rule::Direction>(directionEnum.keyToValue(directionValue, &succeeded));

    if (!succeeded)
        throw ConversionException{};

    return direction;
}