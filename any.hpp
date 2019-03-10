#pragma once
#include <stdexcept>
#include <typeinfo>
#include <string>
class value_base {
public:
	value_base() = default;
	virtual ~value_base() = default;
	virtual value_base *clone() = 0;
	virtual const std::type_info &type() const noexcept = 0;
};
template <typename T>
class value_holder : public value_base {
public:
	T data;
	value_holder() = default;
	value_holder(const T &dat) : data(dat) {}
	virtual ~value_holder() = default;
	virtual value_base *clone() override
	{
		return new value_holder<T>(data);
	}
	virtual const std::type_info &type() const noexcept override
	{
		return typeid(T);
	}
};
class any final {
	value_base *m_data = nullptr;
	value_base *clone() const
	{
		if (m_data != nullptr)
			return m_data->clone();
		else
			return nullptr;
	}

public:
	any() = default;
	any(const any &a) : m_data(a.clone()) {}
	template <typename T>
	any(const T &val) : m_data(new value_holder<T>(val)) {}
	~any()
	{
		delete m_data;
	}
	void assign(const any &a)
	{
		if (&a != this) {
			delete m_data;
			m_data = a.clone();
		}
	}
	template <typename T>
	void assign(const T &val)
	{
		if (m_data != nullptr && m_data->type() == typeid(T))
			static_cast<value_holder<T> *>(m_data)->data = val;
		else {
			delete m_data;
			m_data = new value_holder<T>(val);
		}
	}
	any &operator=(const any &a)
	{
		assign(a);
		return *this;
	}
	template <typename T>
	any &operator=(const T &val)
	{
		assign(val);
		return *this;
	}
	void reset() noexcept
	{
		delete m_data;
		m_data = nullptr;
	}
	void swap(any &a) noexcept
	{
		value_base *tmp = m_data;
		m_data = a.m_data;
		a.m_data = tmp;
	}
	bool has_value() const noexcept
	{
		return m_data != nullptr;
	}
	const std::type_info &type() const noexcept
	{
		if (m_data != nullptr)
			return m_data->type();
		else
			return typeid(void);
	}
	template <typename T>
	T &value()
	{
		value_holder<T> *ptr = dynamic_cast<value_holder<T> *>(m_data);
		if (ptr != nullptr)
			return ptr->data;
		else
			throw std::runtime_error("Bad any cast.");
	}
	template <typename T>
	const T &value() const
	{
		value_holder<T> *const ptr = dynamic_cast<value_holder<T> *const>(m_data);
		if (ptr != nullptr)
			return ptr->data;
		else
			throw std::runtime_error("Bad any cast.");
	}
	template <typename T>
	const T &const_value() const
	{
		return value<T>();
	}
	template <typename T>
	operator T &()
	{
		return value<T>();
	}
	template <typename T>
	operator const T &() const
	{
		return const_value<T>();
	}
};