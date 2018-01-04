
#include <mitsuba/core/spectrum.h>
#include <mitsuba/core/properties.h>

NAMESPACE_BEGIN(mitsuba)

/**
 * \brief Spectrum that takes on a constant value between
 * \c MTS_WAVELENGTH_MIN * and \c MTS_WAVELENGTH_MAX
 */
class UniformSpectrum final : public ContinuousSpectrum {
public:
    UniformSpectrum(const Properties &props) {
        m_value = props.float_("value");
    }

    template <typename Value>
    MTS_INLINE Value eval_impl(Value lambda, mask_t<Value> /* unused */ ) const {
        return Value(m_value) & (lambda >= MTS_WAVELENGTH_MIN) &
               (lambda <= MTS_WAVELENGTH_MAX);
    }

    template <typename Value>
    MTS_INLINE Value pdf_impl(Value lambda, mask_t<Value> /* unused */) const {
        return Value(1.f / (MTS_WAVELENGTH_MAX - MTS_WAVELENGTH_MIN)) &
               (lambda >= MTS_WAVELENGTH_MIN) & (lambda <= MTS_WAVELENGTH_MAX);
    }

    template <typename Value>
    MTS_INLINE std::pair<Value, Value> sample_impl(Value sample, mask_t<Value> /* unused */) const {
        return {
            MTS_WAVELENGTH_MIN + (MTS_WAVELENGTH_MAX - MTS_WAVELENGTH_MIN) * sample,
            Value(m_value * (MTS_WAVELENGTH_MAX - MTS_WAVELENGTH_MIN))
        };
    }

    Float integral() const override {
        return (MTS_WAVELENGTH_MAX - MTS_WAVELENGTH_MIN) * m_value;
    }

    MTS_IMPLEMENT_SPECTRUM()
    MTS_DECLARE_CLASS()

private:
    Float m_value;
};

MTS_IMPLEMENT_CLASS(UniformSpectrum, ContinuousSpectrum)
MTS_EXPORT_PLUGIN(UniformSpectrum, "Uniform spectrum")

NAMESPACE_END(mitsuba)