/**
 * \mainpage Extended Maps
 * \details This is a C++ class that extends the \c std::map class. It adds functionality to disregard "default values" so that the application will not take up more memory than is necessary. It also has a \c NULL that can be used for almost any datatype.
 * \n \n
 * --- Github repository link: <a href="https://github.com/yellowcamper/extended-maps">https://github.com/yellowcamper/extended-maps</a>
 * \n
 * \author Kenneth Michael (Mikey) Neal
 * \date Initial Upload: 5 September 2021
 * \copyright Kenneth Michael (Mikey) Neal (c) 5 September 2021 under GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * \remark This code is still prerelease, do please report any bugs or errors through the github issue tracker.
 */

/**
 * \file extended.h
 * \brief Home of the extended namespace, extended::map, and extended::null.
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
	 * \details This class uses templates to correctly assign NULL values to most types of input.
	 * \param [in] A is the type information.
	 * \param [in] bool is not to be input, the template will not resolve if it is.
	 * 
	 * \param [out] null<A>::value is the value of NULL.
	 * @{
	 */
	
	/**
	 *  \brief \c null\<A\> defines \c NULL for most datatypes.
	 */
	template <class A, bool = std::is_arithmetic<A>::value>
	class null;
	
	/**
	 *  \brief \c null\<A\> defines \c NULL for arithmetic, non-pointer datatypes.
	 */
	template <class A>
	class null <A, true> {
		public:
			static const A value; ///<\c null<A>::value is \c NULL (as defined by retyping \c NULL to the input type) for an arithmetic and non-pointer datatype.
	};
	
	/**
	 *  \brief \c null\<A\> defines \c NULL for non-arithmetic, non-pointer datatypes.
	 */
	template <class A>
	class null <A, false> {
		public:
			static const A value; ///<\c null<A>::value is \c NULL (Assumes that the default constructor is \c NULL) for a non-arithmetic and non-pointer datatype.
	};
	
	/**
	 *  \brief \c null\<A*\> defines \c NULL for pointer datatypes.
	 */
	template <class A>
	class null <A*, false> {
		public:
			static const A value; ///<\c null<A*>::value is \c NULL (uses \c nullptr) for a pointer datatype.
	};
	
	/**
	 *  \brief \c null\<A\&\> defines \c NULL for reference datatypes.
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
	 * \details This class uses \ref null "extended::null<A>" to define a \c default_value for the \c std::map<A, B> unless it is changed in the constructor, then, it does not save items equal to the \c default_value and all values that are not defined will be defined as the \c default_value.
	 * \note All of the \c map<A, B> operators and functions are left unchanged, so there will not be any conflicts, however, many operations will use new operators to make use of this class.
	 * \note A version of this class exists solely because \c std::string does not work like other objects, if you are using this code as a base for an application that acts like \c std::string, I would suggest making a version of the class to suit your needs better.
	 * @{
	 */
	/**
	 * \brief The memory saving version of \c std::map<A, B>.
	 * 
	 * \details This class uses \ref null "extended::null<A>" to define a \c default_value for the \c std::map<A, B> unless it is changed in the constructor, then, it does not save items equal to the \c default_value and all values that are not defined will be defined as the \c default_value.
	 * \note All of the \c map<A, B> operators and functions are left unchanged, so there will not be any conflicts, however, many operations will use new operators to make use of this class.
	 * \note A version of this class exists solely because \c std::string does not work like other objects, if you are using this code as a base for an application that acts like \c std::string, I would suggest making a version of the class to suit your needs better.
	 */
	template <class A, class B>
	class map : public std::map<A, B> {
		protected:
			B default_value = null<B>::value; ///< \c default_value is what stores the values to be ignored, this CANNOT be changed after the constructor to prevent data loss.
		public:
			map();
			map(B);
			template<class... Args> map(B, Args...);
			
			B    operator>> (A);
			void operator() (std::pair<A, B>);
			void operator<< (std::pair<A, B>);
			void operator!  ();
	};
	/**
	 * \brief The memory saving version of \c std::map<A, std::string>.
	 * 
	 * \details This class uses \ref null "extended::null<A>" to define a \c default_value for the \c std::map<A, std::string> unless it is changed in the constructor, then, it does not save items equal to the \c default_value and all values that are not defined will be defined as the \c default_value.
	 * \note All of the \c map<A, std::string> operators and functions are left unchanged, so there will not be any conflicts, however, many operations will use new operators to make use of this class.
	 * \note This version exists solely because \c std::string does not work like other objects, if you are using this code as a base for an application that acts like \c std::string, I would suggest making a version of the class to suit your needs better.
	 */
	template <class A>
	class map <A, std::string> : public std::map<A, std::string> {
		protected:
			std::string default_value = null<std::string>::value; ///< \c default_value is what stores the values to be ignored, this CANNOT be changed after the constructor to prevent data loss.
		public:
			map();
			map(std::string);
			template<class... Args> map(std::string, Args...);
			
			std::string operator>> (A);
			void        operator() (std::pair<A, std::string>);
			void        operator<< (std::pair<A, std::string>);
			void        operator!  ();
	};
	
	/**
	 *  \brief Default constructor
	 *  
	 *  \details This default constructor does nothing except call the \c std::map<A, B> default constructor
	 */
	template <class A, class B>
	map<A, B>::map() : std::map<A, B>() {}
	
	/**
	 *  \brief Constructor.
	 * 
	 *  \param [in] default_val is the \c default_value for this \c extended::map<A, B>.
	 * 
	 *  \details This constructor calls the \c std::map<A, B> default constructor and sets \c default_value to \c default_val.
	 */
	template <class A, class B>
	map<A, B>::map(B default_val) : std::map<A, B>() {
		default_value = default_val;
	}
	
	/**
	 * \brief Constructor.
	 * 
	 * \param [in] default_val is the \c default_value for this \c extended::map<A, B>.
	 * \param [in] args are the arguments to be sent to the \c std::map<A, B> default constructor
	 * 
	 * \details This constructor calls the \c std::map<A, B> constructor, while passing \c args to it and sets \c default_value to \c default_val.
	 */
	template <class A, class B>
	template <class... Args>
	map<A, B>::map(B default_val, Args... args) : std::map<A, B>(args...) {
		default_value = default_val;
	}
	
	
	
	/**
	 * \brief This adds a read only \c operator[] using \c operator>> to "pull" out the value.
	 * 
	 * \param [in] input is the location of the desired value.
	 * \return Returns the value of the location if it exists, however, if it does not exist, it will return the \c default_value.
	 */
	template <class A, class B>
	B map<A, B>::operator>> (A input) {
		if ((*this).count(input) > 0) {
			return (*this).operator[](input);
		}
		return default_value;
	}
	
	/**
	 * \brief This adds a pair to the map if and only if the first value is already in use or the second value is not the \c default_value.
	 * 
	 * \param [in] input is the pair of the location and the desired value.
	 * \return Returns \c void.
	 */
	template <class A, class B>
	void map<A, B>::operator() (std::pair<A, B> input) {
		if (((*this).count(input.first) > 0) || (input.second != default_value)) {
			(*this).operator[](input.first) = input.second;
		}
	}
	
	/**
	 * \brief This adds a pair to the map if and only if the second value is not the \c default_value, if it is \c default_value, then it will also erase the entry.
	 * 
	 * \param [in] input is the pair of the location and the desired value.
	 * \return Returns \c void.
	 */
	template <class A, class B>
	void map<A, B>::operator<< (std::pair<A, B> input) {
		if (input.second != default_value) {
			(*this).operator[](input.first) = input.second;
		} else if ((*this).count(input.first) > 0) {
			auto it = (*this).begin();
			std::advance(it, input.first);
			(*this).erase(it);
		}
	}
	
	/**
	 * \brief This removes any \c default_value from the map by erasing the entry.
	 * 
	 * \return Returns \c void.
	 */
	template <class A, class B>
	void map<A, B>::operator! () {
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
	
	
	/**
	 *  \brief Default constructor
	 *  
	 *  \details This default constructor does nothing except call the \c std::map<A, std::string> default constructor
	 */
	template <class A>
	map<A, std::string>::map() : std::map<A, std::string>() {}
	
	/**
	 *  \brief Constructor.
	 * 
	 *  \param [in] default_val is the \c default_value for this \c extended::map<A, std::string>.
	 * 
	 *  \details This constructor calls the \c std::map<A, std::string> default constructor and sets \c default_value to \c default_val.
	 */
	template <class A>
	map<A, std::string>::map(std::string default_val) : std::map<A, std::string>() {
		default_value = default_val;
	}
	
	/**
	 * \brief Constructor.
	 * 
	 * \param [in] default_val is the \c default_value for this \c extended::map<A, std::string>.
	 * \param [in] args are the arguments to be sent to the \c std::map<A, std::string> default constructor
	 * 
	 * 
	 * \details This constructor calls the \c std::map<A, std::string> constructor, while passing \c args to it and sets \c default_value to \c default_val.
	 */
	template <class A>
	template<class... Args>
	map<A, std::string>::map(std::string default_val, Args... args) : std::map<A, std::string>(args...){
		default_value = default_val;
	}
	
	/**
	 * \brief This adds a pair to the map if and only if the first value is already in use or the second value is not the default_value.
	 * 
	 * \param [in] input is the pair of the location and the desired value.
	 * \return Returns \c void.
	 */
	template <class A>
	std::string map<A, std::string>::operator>> (A input) {
		if ((*this).count(input) > 0) {
			return (*this).operator[](input);
		}
		return default_value;
	}
	
	/**
	 * \brief This adds a pair to the map if and only if the first value is already in use or the second value is not the \c default_value.
	 * 
	 * \param [in] input is the pair of the location and the desired value.
	 * \return Returns \c void.
	 */
	template <class A>
	void map<A, std::string>::operator() (std::pair<A, std::string> input) {
		if (((*this).count(input.first) > 0) || (input.second != default_value)) {
			(*this).operator[](input.first) = input.second;
		}
	}
	
	/**
	 * \brief This adds a pair to the map if and only if the second value is not the \c default_value, if it is \c default_value, then it will also erase the entry.
	 * 
	 * \param [in] input is the pair of the location and the desired value.
	 * \return Returns \c void.
	 */
	template <class A>
	void map<A, std::string>::operator<< (std::pair<A, std::string> input) {
		if (input.second.compare(default_value) != 0) {
			(*this).std::map<A, std::string>::operator[](input.first) = input.second;
		} else if ((*this).std::map<A, std::string>::count(input.first) > 0) {
			auto it = (*this).std::map<A, std::string>::begin();
			std::advance(it, input.first);
			(*this).std::map<A, std::string>::erase(it);
		}
	}
	
	/**
	 * \brief This removes any \c default_value from the map by erasing the entry.
	 * 
	 * \return Returns \c void.
	 */
	template <class A>
	void map<A, std::string>::operator! () {
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