/** @file gsLinearOperator.h

    @brief Simple abstract class for (discrete) linear operators.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): J. Sogn, A. Manzaflaris, C. Hofreither, S. Takacs, C. Hofer
*/
#pragma once

#include <gsCore/gsLinearAlgebra.h>
#include <gsIO/gsOptionList.h>

namespace gismo
{

/// @brief Simple abstract class for discrete operators.
///
/// Simple abstract class for discrete operators.
/// The derived classes have to contain the functions: apply(), cols(), and rows().
///
/// \ingroup Solver
template<class T>
class gsLinearOperator
{
public:

    /// Shared pointer for gsLinearOperator
    typedef memory::shared_ptr<gsLinearOperator> Ptr;

    /// Unique pointer for gsLinearOperator   
    typedef memory::unique_ptr<gsLinearOperator> uPtr;

    /// Identity operator
    static gsIdentityOp<T> Identity(const index_t dim) {return gsIdentityOp<T>(dim); }
    
    virtual ~gsLinearOperator() {}

    /**
     * @brief apply the operator on the input vector and store the result in x
     * @param input Input vector
     * @param x     result vector
     */
    virtual void apply(const gsMatrix<T> & input, gsMatrix<T> & x) const = 0;

    /// Returns the number of rows of the operator
    virtual index_t rows() const = 0;

    /// Returns the number of columns of the operator
    virtual index_t cols() const = 0;

    // NOTE: this is rather inefficient and is only provided for debugging and testing purposes
    void toMatrix(gsMatrix<T>& result)
    {
        GISMO_ASSERT(rows() == cols(),
                     "gsLinearOperator::toMatrix is only implemented for square operators");

        gsMatrix<T> eye = gsMatrix<T>::Identity(cols(), cols());
        this->apply(eye, result);
    }
    
    /// Get the default options as gsOptionList object
    // This implementation provides an empty object
    static gsOptionList defaultOptions()               { return gsOptionList(); }

    /// Set options based on a gsOptionList object
    // This implementation does not read any input
    virtual void setOptions(const gsOptionList & opt)  {}
    
}; // gsLinearOperator

/// @brief Allows an operator to be multiplied with a scalar
///
/// \ingroup Solver
template<class T>
class gsScaledOp : public gsLinearOperator<T>
{
public:
    /// Shared pointer for gsScaledOp
    typedef memory::shared_ptr<gsScaledOp> Ptr;

    /// Unique pointer for gsScaledOp
    typedef memory::unique_ptr<gsScaledOp> uPtr;

    /// Shared pointer for gsLinearOperator
    typedef typename gsLinearOperator<T>::Ptr BasePtr;

    /// Constructor taking a shared pointer to a linear operator and a scalar
    gsScaledOp(const BasePtr & linOp, T scalar = 1) : m_linOp(linOp), m_scalar(scalar)    {}

    /// Make function returning a smart pointer
    static uPtr make(const BasePtr & linOp, T scalar = 1) 
    { return memory::make_unique( new gsScaledOp(linOp, scalar) ); }

    virtual void apply(const gsMatrix<T> & input, gsMatrix<T> & x) const
    {
        m_linOp->apply(input, x);
        x *= m_scalar;
    }

    ///Returns the number of rows in the preconditioner
    index_t rows() const {return m_linOp->rows();}

    ///Returns the number of columns in the preconditioner
    index_t cols() const {return m_linOp->cols();}

private:
    const BasePtr m_linOp;
    const T m_scalar;
}; // gsScaladOp


/// @brief Identity operator
///
/// \ingroup Solver
template<class T>
class gsIdentityOp : public gsLinearOperator<T>
{
public:

    /// Shared pointer for gsIdentityOp
    typedef memory::shared_ptr<gsIdentityOp> Ptr;

    /// Unique pointer for gsIdentityOp   
    typedef memory::unique_ptr<gsIdentityOp> uPtr;
    
    /// Constructor taking the dimension of the identity operator
    gsIdentityOp(index_t dim) : m_dim(dim) {}

    /// Make function returning a smart pointer
    static uPtr make(index_t dim) { return memory::make_unique( new gsIdentityOp(dim) ); }

    void apply(const gsMatrix<T> & input, gsMatrix<T> & x) const
    {
        x = input;
    }

    index_t rows() const {return m_dim;}

    index_t cols() const {return m_dim;}

private:
    const index_t m_dim;
};

} // namespace gismo
