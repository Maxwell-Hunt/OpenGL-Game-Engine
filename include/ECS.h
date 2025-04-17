#include <typeindex>
#include <memory>
#include <unordered_map>
#include <unordered_set>

using EntityId = std::size_t;

class ComponentGroupBase {
public:
    virtual ~ComponentGroupBase() = default;
};

template <typename T>
class ComponentGroup : public ComponentGroupBase {
public:
    void addToEntity(EntityId entity, T* component) {
        mData[entity] = component;
    }

    T* getComponent(EntityId entity) {
        return mData.at(entity);
    }
private:
    std::unordered_map<EntityId, T*> mData;
};

class ECS;

class System {
public:
    virtual ~System() = default;
    virtual void run(ECS& ecs) = 0;
};

class ECS {
public:
    EntityId createEntity() {
        return nextEntity++;
    }

    std::size_t numEntities() {
        return nextEntity;
    }

    template <typename T>
    void addComponentToEntity(EntityId entity, T* component) {
        std::type_index index = std::type_index(typeid(T));
        if(!mGroups.count(index)) {
            mGroups[index] = std::unique_ptr<ComponentGroupBase>(new ComponentGroup<T>());
        }
        static_cast<ComponentGroup<T>*>(mGroups[index].get())->addToEntity(entity, component);
        mEntityComponents[entity].insert(index);
    }

    template <typename T>
    T* getComponent(EntityId entity) {
        std::type_index index = std::type_index(typeid(T));
        return static_cast<ComponentGroup<T>*>(mGroups[index].get())->getComponent(entity);
    }

    template <typename FirstComponent, typename... Rest>
    bool hasComponents(EntityId entity) {
        std::type_index index = std::type_index(typeid(FirstComponent));
        
        if (!mEntityComponents[entity].count(index)) {
            return false;
        }

        if constexpr (sizeof...(Rest) > 0) {
            return hasComponents<Rest...>(entity);
        }
        return true;
    }

    void addSystem(System* system) {
        systems.insert(system);
    }

    void removeSystem(System* system) {
        systems.erase(system);
    }

    void runSystems() {
        for(System* system : systems) {
            system->run(*this);
        }
    }

private:
    EntityId nextEntity = 0;
    std::unordered_map<std::type_index, std::unique_ptr<ComponentGroupBase>> mGroups;
    std::unordered_map<EntityId, std::unordered_set<std::type_index>> mEntityComponents;
    std::unordered_set<System*> systems;
};

/*
-----------------------------COMMENTS-------------------------------

template<typename... T>
std::vector<EntityID> getEntitiesWithComponents() {
    std::vector<std::unordered_set<EntityID>> entitySets = {getEntitySet<T>()...};

    // Sort by size for efficient intersection
    std::sort(entitySets.begin(), entitySets.end(),
              [](const auto& a, const auto& b) {
                  return a.size() < b.size();
              });

    std::vector<EntityID> result;
    for (const EntityID& id : entitySets[0]) {
        if (std::all_of(entitySets.begin() + 1, entitySets.end(),
                        [&id](const std::unordered_set<EntityID>& set) {
                            return set.count(id);
                        })) {
            result.push_back(id);
        }
    }

    return result;
}
*/