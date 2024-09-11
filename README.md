# computor_v1
## 1. Overview
The program that solves a polynomial second or lower degree equation.

## 2. How to use
### 2-1) Clone and compile
```shell
git clone git@github.com:ak0327/42_computor_v1.git
make -C 42_computor_v1
```

### 2-2) Execute
```shell
./computor <equation>

# Example:
$> ./computor "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
Reduced form     : 9.3 * X^2 - 4 * X - 4 = 0
Polynomial degree: 2
Discriminant is positive, the two solutions are:
 0.905239
-0.475131

$> ./computor "5 * X^0 + 4 * X^1 = 4 * X^0"
Reduced form     : 4 * X + 1 = 0
Polynomial degree: 1
The solution is:
-0.25

$> ./computor "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"
Reduced form     : 5.6 * X^3 + 6 * X - 5 = 0
Polynomial degree: 3
The polynomial degree is strictly greater than 2, I can't solve.
```

## 3. Confirmed Environments
* Ubuntu 22.04.2 LTS (ARM64)
* MacOS OS Ventura 13.5 (ARM64)


## 4. References
