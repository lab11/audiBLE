/* Parse audiBLE advertisements */

var parse_advertisement = function (advertisement, cb) {

    if (advertisement.localName === 'audiBLE') {
        if (advertisement.manufacturerData) {
            // Need at least 3 bytes. Two for manufacturer identifier and
            // one for the service ID.
            if (advertisement.manufacturerData.length >= 3) {
                // Check that manufacturer ID and service byte are correct
                var manufacturer_id = advertisement.manufacturerData.readUIntLE(0, 2);
                var service_id = advertisement.manufacturerData.readUInt8(2);
                if (manufacturer_id == 0x02E0 && service_id == 0x1E) {
                    // This is a audiBLE packet
                    if (advertisement.manufacturerData.length == 5) {
                        var pir = advertisement.manufacturerData.slice(3);

                        var sound_level           = pir.readUInt16(0); // Value should be from 0-1023

                        var out = {
                            device: 'Blink',
                            sound_level:           sound_level
                        };

                        cb(out);
                        return;
                    }
                }
            }
        }
    }

    cb(null);
}


module.exports = {
    parseAdvertisement: parse_advertisement
};
