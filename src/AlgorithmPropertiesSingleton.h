#ifndef ALGORITHMPROPERTIESSINGLETON_H
#define ALGORITHMPROPERTIESSINGLETON_H

#include <QString>

class AlgorithmPropertiesSingleton
{
public:
        static AlgorithmPropertiesSingleton& getInstance()
        {
                static AlgorithmPropertiesSingleton theSingleInstance;
                return theSingleInstance;
        }

        void clearCommonContainer();

        void setAlgSteps(QString steps) { this->m_algSteps = steps; }
        QString getAlgSteps() const { return m_algSteps; }
        QString & getAlgStepsRef() { return m_algSteps; }

        void setNumbeOfOperations(quint32 ops) { this->m_NumbeOfOperations = ops; }
        quint32 getNumbeOfOperations() const { return m_NumbeOfOperations; }
        quint32 & getNumbeOfOperationsRef() { return m_NumbeOfOperations; }

private:
        AlgorithmPropertiesSingleton(){}
        AlgorithmPropertiesSingleton(const AlgorithmPropertiesSingleton& root);
        AlgorithmPropertiesSingleton& operator=(const AlgorithmPropertiesSingleton&);

        QString m_algSteps;
        quint32 m_NumbeOfOperations;
};

#endif // ALGORITHMPROPERTIESSINGLETON_H
