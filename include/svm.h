#ifndef SVM_H
#define SVM_H

#include <memory>
#include <opencv2/core/core.hpp>
#include "clany/clany_defs.h"
#include "libsvm.h"

_CLANY_BEGIN
namespace ml {

using SVMNode      = svm_node;
using SVMProblem   = svm_problem;
using SVMModel     = svm_model;

struct SVMParameter : svm_parameter {
    SVMParameter& operator= (const cv::SVMParams& params) {
        svm_type    = params.svm_type;
        kernel_type = params.kernel_type;
        C           = params.C;
        coef0       = params.coef0;
        degree      = static_cast<int>(params.degree);
        gamma       = params.gamma;
        nu          = params.nu;
        p           = params.p;
        eps         = params.term_crit.epsilon;
        max_iter    = params.term_crit.max_iter;

        return *this;
    }
};

struct SVMModelDeletor {
    void operator()(SVMModel* ptr) {
        svm_free_and_destroy_model(&ptr);
    }
};


class SVM {
public:
    // SVM type
    enum { C_SVC = 100, NU_SVC = 101, ONE_CLASS = 102, EPS_SVR = 103, NU_SVR = 104 };

    // Kernel type
    enum { LINEAR = 0, POLY = 1, RBF = 2, SIGMOID = 3};

    void train(const cv::Mat& train_data, cv::InputArray labels, const SVMParameter& params);
    void train(const cv::Mat& train_data, cv::InputArray labels,
               int svm_type = C_SVC, int kernel_type = RBF, int k_fold = 10);

    double predict(cv::InputArray sample) const;
    double predict(const SVMNode& sample) const;

    double predict(cv::InputArray sample, vector<double>& vals, bool return_prob_val = false) const;
    double predict(const SVMNode& sample, vector<double>& vals, bool return_prob_val = false) const;

    auto getParams() const -> SVMParameter {
        return params;
    }

private:
    cv::Mat train_mat;
    cv::Mat label_mat;
    SVMParameter params;
    unique_ptr<SVMModel, SVMModelDeletor> model_ptr;
};

} // End namespace ml
_CLANY_END

#endif // SVM_H