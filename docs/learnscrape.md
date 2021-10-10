# learnscrape
Technical Code Specification Manual

## Roadmap for Supervised Learning via Multivariate Polynomial Regression
1. Scrape training dataset with `m` entries `{X^i, y^i}` where `i = 0,...,(m-1)` and each `X^i` encapsulates `n` features.
2. Utilise feature scaling 
3. Configure the prediction model `hypothesisFunction()` as parametrised by the `{thetaParameter}` which are to be determined.
4. Formulate an ansatz for `{thetaParameter}` and compute the mean-squared error `costFunction()` for this initial guess.
5. Perform a multi-parameter optimisation of the `costFunction()` upon computing the `{thetaParameter}` derivative gradients.
6. Repeat the minimisation procedure for `costFunction` until a convergence criterion is satisfied.
7. Evaluate some predictions using interpolation/extropolation schemes as specified by the user.
8. Produce an animate report designed to demonstrate the algorithmic workflow as well as analysis of the results.



| Term | Symbol | Definition |
|------|--------|------------|
| `costFunction`      | x      |  x         |
 

## Intuition

### Machine Learning

#### Supervised Learning


