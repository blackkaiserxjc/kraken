#pragma once

namespace kr {
namespace utility {

struct ioc_container::Id
{
};

template <typename T>
struct ioc_container::TypeId
{
    static ioc_container::Id id;
};

template <typename T>
ioc_container::Id ioc_container::TypeId<T>::id;

template <typename T>
struct ioc_container::Holder : public ioc_container::IHolder
{
    std::shared_ptr<T> instance;

    template <typename... Args>
    explicit Holder(std::shared_ptr<T> instance)
        : instance(std::move(instance))
    {
        id = &ioc_container::TypeId<T>::id;
    }

    void initialize(const ioc_container::Id &) override
    {
    }

    void destroy(const ioc_container::Id &) override
    {
    }
};

template <typename T>
struct ioc_container::IocHolder : ioc_container::Holder<T>
{
    template <typename... Args>
    explicit IocHolder(Args &&...args)
        : ioc_container::Holder<T>(std::forward<Args>(args)...)
    {
    }

    void initialize(const ioc_container::Id &) override
    {
        this->instance->on_initialize();
    }

    void destroy(const ioc_container::Id &) override
    {
        this->instance->on_destroy();
    }
};

template <typename T, typename... Args>
void ioc_container::bind(Args &&...args)
{
    bind_as<T, T>(std::forward<Args>(args)...);
}

template <typename I, typename T, typename... Args>
void ioc_container::bind_as(Args &&...args)
{
    auto creator = [](auto &&...args) -> std::shared_ptr<IHolder> {
        using HolderType = std::conditional_t<std::is_base_of_v<ioc, I>, IocHolder<I>, Holder<I>>;
        return std::make_shared<HolderType>(std::make_shared<T>(std::forward<decltype(args)>(args)...));
    };
    creators_[&TypeId<I>::id] = std::bind(creator, std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<T> ioc_container::load()
{
    if (auto obj_iter = objects_.get<1>().find(&TypeId<T>::id);
        obj_iter != objects_.get<1>().end())
    {
        auto holder = std::static_pointer_cast<Holder<T>>(*obj_iter);
        return holder->instance;
    }

    if (auto creator_iter = creators_.find(&TypeId<T>::id);
        creator_iter != creators_.end())
    {
        auto holder = std::static_pointer_cast<Holder<T>>(creator_iter->second());
        holder->initialize(Id());
        return holder->instance;
    }
    return nullptr;
}

} // namespace utility
} // namespace kr