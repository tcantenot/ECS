#include "VariableManager.hpp"

template <typename T>
void VariableManager::setValue(std::string const & name, T const & value)
{
    auto & variableHashMap = getValueMap<T>();
    variableHashMap[name] = value;
}

template <typename T>
T const & VariableManager::getValue(std::string const & name)
{
    static_assert(std::is_default_constructible<T>::value,
                  "T must be DefaultConstructible");

    auto & variableHashMap = getValueMap<T>();

    auto it = variableHashMap.find(name);

    if(it == variableHashMap.end())
    {
        variableHashMap.insert(std::make_pair(name, T()));
        it = variableHashMap.find(name);
    }
    
    return it->second;
}


template <typename T>
T VariableManager::getValue(std::string const & name, T const & defaultValue)
{
    static_assert(std::is_copy_constructible<T>::value,
                  "T must be CopyConstructible");

    auto & variableHashMap = getValueMap<T>();

    auto it = variableHashMap.find(name);

    if(it == variableHashMap.end())
    {
        return defaultValue;
    }
    
    return it->second;
}


template <typename T>
VariableManager::VariableHashMap<T> & VariableManager::getValueMap()
{
    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    auto const it = m_values.find(index);

    if(it == m_values.end())
    {
        m_values.insert(
            std::make_pair(index, std::make_shared<VariableHashMap<T>>()));
    }

    return *std::static_pointer_cast<VariableHashMap<T>>(m_values[index]).get();
}

