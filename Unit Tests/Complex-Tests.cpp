#include <iostream>
#include <random>
#include "ORCAMath/ORCAMath.h"

using namespace ORCA;

#define RAND_LOWER 10000
#define RAND_UPPER -RAND_LOWER

int main(int argc, const char * argv[]) {
    // insert code here...

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_real_distribution<float> distribution(RAND_LOWER,RAND_UPPER);

    float a = distribution(generator);
    float b = distribution(generator);

    /* Show constructors */

    Complex<float> defaultConstructor;
    assert((defaultConstructor.re() == 0) && (defaultConstructor.im() == 0));

    Complex<float> argConstructor(a,b);
    assert((argConstructor.re() == a) && (argConstructor.im() == b));

    Complex<float> listConstructor = {a,b};
    assert((listConstructor.re() == a) && (listConstructor.im() == b));

    Complex<double> typeCastConstructor1 = {a,b};
    Complex<float> typeCastConstructor2 = typeCastConstructor1;
    assert((typeCastConstructor1.re() == a) && (typeCastConstructor1.im() == b));
    assert((typeCastConstructor2.re() == a) && (typeCastConstructor2.im() == b));

    assert(typeCastConstructor1.conj().re() == typeCastConstructor1.re());
    assert(typeCastConstructor1.conj().im() == -typeCastConstructor1.im());

    /* Show caught construction exception */

    try {
        Complex<float> exceptConstructor = {1,2,3};
    } catch (ORCAExcept::ORCAException exception) {
        assert(exception == ORCA_BAD_DIMENSIONS);
    }

    /* Show math operation correctness */

    float c = distribution(generator);
    float d = distribution(generator);
    float e = distribution(generator);

    Complex<float> math1 = {a,b};
    Complex<float> math2 = {c,d};

    /* Addition */

    Complex<float> additionResult = math1 + math2;
    Complex<float> additionResult2 = math1 + e;

    assert(additionResult.real() == (a + c));
    assert(additionResult.imag() == (b + d));
    assert(additionResult2.real() == (a + e));
    assert(additionResult2.imag() == b);
    assert((math1 + e) == (e + math1));

    /* Subtraction */

    Complex<float> subtractionResult = math1 - math2;
    Complex<float> subtractionResult2 = math1 - e;
    Complex<float> subtractionResult3 = e - math1;

    assert(subtractionResult.real() == (a - c));
    assert(subtractionResult.imag() == (b - d));
    assert(subtractionResult2.re() == (a - e));
    assert(subtractionResult2.im() == b);
    assert(subtractionResult2 == -subtractionResult3);
    assert((math1 - math2) == -(math2 - math1));

    /* Multiplication */

    Complex<float> multiplicationResult = math1 * math2;
    Complex<float> multiplicationResult1 = math1 * e;
    Complex<float> multiplicationResult2 = e * math1;

    assert(multiplicationResult.real() == ((a * c) - (b * d)));
    assert(multiplicationResult.imag() == ((a * d) + (b * c)));
    assert(multiplicationResult1.real() == (e * a));
    assert(multiplicationResult1.im() == (e * b));
    assert(multiplicationResult1 == multiplicationResult2);

    /* Equality */

    Complex<float> realOnly = e;

    assert(realOnly == e);
    assert(e == realOnly);


    /* Printing */

    std::cout << multiplicationResult1 << std::endl;
    std::cout << &multiplicationResult1 << std::endl;

    return 0;
}
