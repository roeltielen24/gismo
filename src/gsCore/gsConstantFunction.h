/** @file gsConstantFunction.h

    @brief Provides declaration of ConstantFunction class.

    This file is part of the G+Smo library. 

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    
    Author(s): C. Hofreither
*/

#pragma once

#include <gsCore/gsLinearAlgebra.h>
#include <gsCore/gsFunction.h>

namespace gismo
{

/** 
    @brief Class defining a constant function

    \tparam T value type

    \ingroup function
    \ingroup Core
*/

template <class T>
class gsConstantFunction : public gsFunction<T>
{
public:
    gsConstantFunction() { }

    explicit gsConstantFunction(const gsVector<T>& val, int domainDim = 1)
        : m_val(val), m_domainDim(domainDim)
    { }


    explicit gsConstantFunction(T x, int domainDim  = 1)
        : m_domainDim(domainDim)
    {
        m_val.resize(1);
        m_val(0) = x;
    }

    gsConstantFunction(T x, T y, int domainDim)
        : m_domainDim(domainDim)
    {
        m_val.resize(2);
        m_val(0) = x;
        m_val(1) = y;
    }

    gsConstantFunction(T x, T y, T z, int domainDim)
        : m_domainDim(domainDim)
    {
        m_val.resize(3);
        m_val(0) = x;
        m_val(1) = y;
        m_val(2) = z;
    }
    gsConstantFunction(T x, T y, T z, T n,  int domainDim)
        : m_domainDim(domainDim)
    {
        m_val.resize(4);
        m_val(0) = x;
        m_val(1) = y;
        m_val(2) = z;
        m_val(3) = n;
    }

    virtual gsConstantFunction * clone() const { return new gsConstantFunction(*this); }

    virtual int domainDim() const   { return m_domainDim ; }

    virtual int targetDim() const   { return m_val.rows(); }

    const gsVector<T> & value() const { return m_val;}

    T value(size_t i = 0) const { return m_val[i];}

    void setValue(T val) { m_val.setConstant(val);}

    void setValue(const gsVector<T> & val) { m_val = val;}

    virtual void eval_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
    {
        GISMO_ASSERT(u.rows() == m_domainDim, "Wrong domain dimension "<< u.rows()
                                              << ", expected "<< m_domainDim);
        result = m_val.rowwise().replicate( u.cols() );
    }

    virtual void deriv_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
    {
        GISMO_ASSERT(u.rows() == m_domainDim, "Wrong domain dimension "<< u.rows()
                                              << ", expected "<< m_domainDim);
        result = gsMatrix<T>::Zero( this->targetDim(), this->domainDim() * u.cols() );
    }

    virtual void deriv2_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
    {
        GISMO_ASSERT(u.rows() == m_domainDim, "Wrong domain dimension "<< u.rows()
                                              << ", expected "<< m_domainDim);
        result = gsMatrix<T>::Zero( (this->domainDim()*(this->domainDim()+1))/2,
                                    this->targetDim()*u.cols() );
    }

    /// Prints the object as a string.
    virtual std::ostream &print(std::ostream &os) const
    {
        os << m_val; 
        return os; 
    }
  
private:

    gsVector<T> m_val;

    int m_domainDim;
};

}
