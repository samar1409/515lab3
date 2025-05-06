#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class DecisionTree {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        if (x[2] <= 3.5) {
                            if (x[2] <= 1.5) {
                                return 1;
                            }

                            else {
                                if (x[7] <= 3.0) {
                                    return 2;
                                }

                                else {
                                    return 0;
                                }
                            }
                        }

                        else {
                            return 3;
                        }
                    }

                protected:
                };
            }
        }
    }