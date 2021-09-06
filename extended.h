/**
 *  \file extended.h
 *  \brief Home of the extended namespace, extended::map, and extended::null.
 *  \copyright Kenneth Michael (Mikey) Neal (c) 5 September 2021 under GNU GENERAL PUBLIC LICENSE Version 3 (c) 29 June 2007
 */
#ifndef __EXTENDED_H__
#define __EXTENDED_H__

#include <map>
#include <iterator>
#include <string>
#include <utility>
#include <type_traits>

namespace extended {
	/**
	 * \defgroup null extended::null
	 * 
	 * \brief The type independent null class
	 * \details This class uses templates to correctly assign NULL values to any type of input.
	 * \param [in] A is the type information.
	 * \param [in] bool is not to be input, the template will not resolve if it is.
	 * 
	 * \param [out] null<A>::value is the value of NULL.
	 * @{
	 */
	
	/**
	 *  \brief \c null<A> defines \c NULL for most datatypes.
	 */
	template <class A, bool = std::is_arithmetic<A>::value>
	class null;
	
	/**
	 *  \brief \c null<A> defines \c NULL for arithmetic, non-pointer datatypes.
	 */
	template <class A>
	class null <A, true> {
		public:
			static const A value; ///<\c null<A>::value is \c NULL (as defined by retyping \c NULL to the input type) for an arithmetic and non-pointer datatype.
	};
	
	/**
	 *  \brief \c null<A> defines \c NULL for non-arithmetic, non-pointer datatypes.
	 */
	template <class A>
	class null <A, false> {
		public:
			static const A value; ///<\c null<A>::value is \c NULL (Assumes that the default constructor is \c NULL) for a non-arithmetic and non-pointer datatype.
	};
	
	/**
	 *  \brief \c null<A> defines \c NULL for pointer datatypes.
	 */
	template <class A>
	class null <A*, false> {
		public:
			static const A value; ///<\c null<A*>::value is \c NULL (uses \c nullptr) for a pointer datatype.
	};
	
	/**
	 *  \brief \c null<A> defines \c NULL for reference datatypes.
	 */
	template <class A>
	class null <A&, false> {
		public:
			static const A value; ///<\c null<A&>::value is \c NULL (uses \c nullptr) for a reference datatype.
	};
	
	template <class A>
	const A null<A, true >::value = (A)NULL;
	
	template <class A>
	const A null<A, false>::value = A();
	
	template <class A>
	const A null<A*, false>::value = nullptr;
	
	template <class A>
	const A null<A&, false>::value = nullptr;
	/**@}
	 * \defgroup map extended::map
	 * 
	 * \brief The memory saving version of \c std::map<A, B>.
	 * 
	 * \details This class uses \ref null "extended::null<A>" to define a \c default_value for the \c std::map<A, B> unless it is changed in the constructor.
	 * @{
	 */
	template <class A, class B>
	class map : public std::map<A, B> {
		protected:
			B default_value = null<B>::value;
		public:
			/**
			 *  \brief Default constructor
			 *  
			 *  \details This default constructor does nothing except call the \c std::map default constructor
			 */
			map() : std::map<A, B>() {}
			map(B);
			template<class... Args> map(B, Args... args);
			
			B    operator>> (A);
			void operator() (std::pair<A, B>);
			void operator<< (std::pair<A, B>);
			void operator!  ();
	};
	
	template <class A>
	class map <A, std::string> : public std::map<A, std::string> {
		protected:
			std::string default_value = null<std::string>::value;
		public:
			/**
			 *  \brief Default constructor
			 *  
			 *  \details This default constructor does nothing except call the \c std::map default constructor
			 */
			map() : std::map<A, std::string>() {}
			map(std::string);
			template<class... Args> map(std::string, Args... args);
			
			std::string operator>> (A);
			void        operator() (std::pair<A, std::string>);
			void        operator<< (std::pair<A, std::string>);
			void        operator!  ();
	};
	
	template <class A, class B>
	template <class... Args>
	map<A, B>::map(B default_val, Args... args) : std::map<A, B>(args...) {
		default_value = default_val;
	}
	
	/**
	 *  \brief Constructor.
	 * 
	 *  \param [in] default_val is the \c default_value for this \c extended::map<A, B>.
	 * 
	 *  \details This default constructor calls the \c std::map default constructor and sets \c default_value to \c default_val.
	 */
	template <class A, class B>
	map<A, B>::map(B default_val) : std::map<A, B>() {
		default_value = default_val;
	}
	
	template <class A, class B>
	B map<A, B>::operator>> (A input) { // adds a read only operator[] using operator>> to "pull" out the value.
		if ((*this).count(input) > 0) {
			return (*this).operator[](input); // if exists return like normal
		}
		return default_value; // else return default
	}
	
	template <class A, class B>
	void map<A, B>::operator() (std::pair<A, B> input) { // add a pair to the map if and only if the first value is already in use or the second value is not the default_value, keeps default_value's
		if (((*this).count(input.first) > 0) || (input.second != default_value)) {
			(*this).operator[](input.first) = input.second;
		}
	}
	
	template <class A, class B>
	void map<A, B>::operator<< (std::pair<A, B> input) { // add a pair to the map if and only if the first value is already in use or the second value is not the default_value, removes default_value's
		if (input.second != default_value) {
			(*this).operator[](input.first) = input.second;
		} else if ((*this).count(input.first) > 0) {
			auto it = (*this).begin();
			std::advance(it, input.first);
			(*this).erase(it);
		}
	}
	
	template <class A, class B>
	void map<A, B>::operator! () { // Gets rid of default values
		uint64_t temp = 0;
		for (auto it = (*this).cbegin(); it != (*this).cend(); ) {
			if ((*it).second == default_value) {
				temp = std::distance((*this).cbegin(), it);
				(*this).erase(it);
				it = (*this).cbegin();
				std::advance(it, temp);
			} else {
				std::advance(it, 1);
			}
		}
	}
	
	template <class A>
	template<class... Args>
	map<A, std::string>::map(std::string default_val, Args... args) : std::map<A, std::string>(args...){
		default_value = default_val;
	}
	
	template <class A>
	map<A, std::string>::map(std::string default_val) : std::map<A, std::string>() {
		default_value = default_val;
	}
	
	template <class A>
	std::string map<A, std::string>::operator>> (A input) { // adds a read only operator[] using operator>> to "pull" out the value.
		if ((*this).count(input) > 0) {
			return (*this).operator[](input); // if exists return like normal
		}
		return default_value; // else return default
	}
	
	template <class A>
	void map<A, std::string>::operator() (std::pair<A, std::string> input) { // add a pair to the map if and only if the first value is already in use or the second value is not the default_value, keeps default_value's
		if (((*this).count(input.first) > 0) || (input.second != default_value)) {
			(*this).operator[](input.first) = input.second;
		}
	}
	
	template <class A>
	void map<A, std::string>::operator<< (std::pair<A, std::string> input) { // add a pair to the map if and only if the first value is already in use or the second value is not the default_value, removes default_value's
		if (input.second.compare(default_value) != 0) {
			(*this).std::map<A, std::string>::operator[](input.first) = input.second;
		} else if ((*this).std::map<A, std::string>::count(input.first) > 0) {
			auto it = (*this).std::map<A, std::string>::begin();
			std::advance(it, input.first);
			(*this).std::map<A, std::string>::erase(it);
		}
	}
	
	template <class A>
	void map<A, std::string>::operator! () { // Gets rid of default values
		uint64_t temp = 0;
		for (auto it = (*this).cbegin(); it != (*this).cend(); ) {
			if ((*it).second.compare(default_value) == 0) {
				temp = std::distance((*this).cbegin(), it);
				(*this).erase(it);
				it = (*this).cbegin();
				std::advance(it, temp);
			} else {
				std::advance(it, 1);
			}
		}
	}
	///@}
}
#endif