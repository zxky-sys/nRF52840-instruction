FROM qmkfm/qmk_firmware

RUN git clone --recurse-submodules -b nrf52 https://github.com/sekigon-gonnoc/qmk_firmware.git

RUN wget https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v15.x.x/nRF5_SDK_15.3.0_59ac345.zip && \
    unzip nRF5_SDK_15.3.0_59ac345.zip && rm nRF5_SDK_15.3.0_59ac345.zip