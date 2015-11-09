#ifndef FUNCTIONPROPERTIESHOLDER_H
#define FUNCTIONPROPERTIESHOLDER_H

#include <QList>

class FunctionPropertiesHolder
{
public:
    FunctionPropertiesHolder();

    void setGamma(quint32 gamma) { this->m_gamma = gamma; }
    quint32 getGamma() const { return m_gamma; }

    void setParenthesesCount(quint32 parenthesesCount) { this->m_parenthesesCount = parenthesesCount; }
    quint32 getParenthesesCount() const { return m_parenthesesCount; }

    void setVariablesOutsideParentheses(const QList<int> variablesOutsideParentheses) { this->m_variablesOutsideParentheses = variablesOutsideParentheses; }
    QList<int> getVariablesOutsideParentheses() const { return m_variablesOutsideParentheses; }


private:
    quint32 m_gamma;
    quint32 m_parenthesesCount;
    QList<int> m_variablesOutsideParentheses;
};

#endif // FUNCTIONPROPERTIESHOLDER_H
