#ifndef HYBRID_ECS_VARIABLE_MANAGER_HPP
#define HYBRID_ECS_VARIABLE_MANAGER_HPP

#include <string>
#include <memory>
#include <type_traits>

#include <Managers/Manager.hpp>
#include <Utils/Types.hpp>
#include <Utils/TypeIndexHash.hpp>

namespace Hybrid
{
    namespace ECS
    {
        class VariableManager : public Manager
        {
            template <typename T>
            using VariableHashMap = Hash<std::string, T>;

            public:
                VariableManager();
                virtual ~VariableManager();

                template <typename T>
                void setValue(std::string const & name, T const & value);

                template <typename T>
                T const & getValue(std::string const & name);
                
                template <typename T>
                T getValue(std::string const & name, T const & defaultValue);

                virtual void onRegistered() override;

            protected:
                template <typename T>
                VariableHashMap<T> & getValueMap();

            private:
                Hash<TypeIndexHash, std::shared_ptr<void>> m_values;
        };

        #include "VariableManager.inl"
    }
}

#endif //HYBRID_ECS_VARIABLE_MANAGER_HPP
