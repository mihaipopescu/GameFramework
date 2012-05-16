#ifndef SINGLETON_H
#define SINGLETON_H

/*!
 * \file singleton.h
 * \brief Singleton pattern.
 */

/*!
  \def SINGLETON_IMPL(T)
  \brief This macro is used in the singleton implementation file to initialize the static instance.
  Example: \code SINGLETON_IMPL(MyClass); \endcode
 */
#define SINGLETON_IMPL(T) template<> T* Singleton<T>::myInstance = NULL;

/*!
 * \def SINGLETON_DECL(T)
 * \brief This macro is used to declare a protected default-ctor and the friendship with the singleton class. 
 */
#define SINGLETON_DECL(T) protected: T(); friend class Singleton<T>; private:

//! Template class for singleton pattern.
/*!
 * Base class used to create singletons. \n
 * Usage example: \code class MyClass : public Singleton<MyClass>
 * {
 *  SINGLETON_DECL(MyClass)
 *  ...
 * } \endcode
 * \note The derived template class T must not declare any other c-tor.
 */
template <class T>
class Singleton
{
public:
    //! Creates the singleton instance
    static void   Create()  { myInstance = new T(); }
    //! Retrieves the singleton instance
    static T*     Get()     { return myInstance; }
    //! Destroys the singleton instance and sets it to NULL
    static void   Destroy() { delete myInstance; myInstance = NULL; }

protected:
    static T* myInstance; //!< The templated instance
};


#endif