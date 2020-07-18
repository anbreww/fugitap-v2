/**
 * Beer.h
 * 
 * Manages Beer information (Name, Style, ABV etc) and flow meter.
 * Reads the flow meter input to get status of beer pours, triggers a callback
 * when a beer starts pouring and a pour finishes (includes the time and volume)
 */

#include <Arduino.h>
#include <FlowMeter.h>

class Beer
{
    public:
        Beer(FlowMeter& meter, uint8_t pouring_gpio);
        void    set_tap(int8_t tap_no);
        void    set_full_vol(double);
        void    set_poured(double);
        void    set_img(String new_img);
        void    refresh(void);
        String  name(void),
                type(void),
                abv(void),
                ibu(void),
                glass_img(void),
                og(void);
        uint8_t tap(void);
        double  full_vol(void),
                volume(void);
        uint32_t last_updated(void);
        bool    is_pouring(void);
        void    begin(void);
    private:
        int8_t _tap_no;
        String  _name,
                _style,
                _og,
                _untappd_url,
                _glass_url,
                _abv,
                _ibu,
                _glass_img;
        double  _full_vol,
                _poured;
        uint32_t _last_updated;
        uint32_t _last_pouring;
        FlowMeter& _flow_meter;
        uint8_t _pouring_gpio;
        void loadSamples(void);
};