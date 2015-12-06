#ifndef ALGORITHMPROPERTIESSINGLETON_H
#define ALGORITHMPROPERTIESSINGLETON_H

#include "GraphsTypesHelper.h"
#include "GraphsOperationsHelper.h"

#define ALGS_NUMBER 8

class AlgorithmPropertiesSingleton
{
    typedef QList<int> (*AlgFuncPtr) (UndirectedGraphType graph);

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
        QList<int> & getMVCContainterRef() { return m_MVC; }

        const AlgFuncPtr * getAlgsListRef() { return m_algsList; }

private:
        AlgorithmPropertiesSingleton(){}
        AlgorithmPropertiesSingleton(const AlgorithmPropertiesSingleton& root);
        AlgorithmPropertiesSingleton& operator=(const AlgorithmPropertiesSingleton&);

        QString m_algSteps;
        quint32 m_NumbeOfOperations;
        QList<int> m_MVC;

        static const AlgFuncPtr m_algsList[ALGS_NUMBER];
};

#endif // ALGORITHMPROPERTIESSINGLETON_H
