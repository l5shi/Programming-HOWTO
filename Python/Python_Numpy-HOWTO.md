# Basics

`np.diag(s)`: diagonal matrix

Operation| Functions
---------|----------
 Matrix Multipy | `np.dot(U, V)`
 Vector Outer Product | `np.outer(U[:, i], V[i, :])` 
 Vector Inner Product | `np.inner(U[:, i], V[i, :])` 

 `np.sum(matrix, axis=0)`

# SVD

[**How much faster is a truncated singular value decomposition?**](http://blog.explainmydata.com/2016/01/how-much-faster-is-truncated-svd.html?m=1)
a full SVD with `numpy.linalg.svd` (which uses LAPACK under the hood)

an exact low-rank SVD with `sklearn.decomposition.TruncatedSVD` using ARPACK

an approximate low-rank SVD also with `sklearn.decomposition.TruncatedSVD` but using a faster randomized solver