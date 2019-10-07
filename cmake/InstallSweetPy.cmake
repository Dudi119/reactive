if (NOT SweetPy_FOUND)

    ExternalProject_Add(SweetPy
            GIT_REPOSITORY      https://github.com/Dudi119/sweetPy
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && cmake -DsweetPy_3RD_PARTY_DIR:STRING=<INSTALL_DIR> -DOUTPUT_DIR=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} .
            BUILD_COMMAND       cd <SOURCE_DIR> && make
            INSTALL_COMMAND     cd <SOURCE_DIR> && make install
            TEST_COMMAND        ""
            )

    if(SWEETPY_DEPEND_LIST)
        add_dependencies(SweetPy ${SWEETPY_DEPEND_LIST})
    endif()
    set(REACTIVE_DEPEND_LIST ${REACTIVE_DEPEND_LIST} SweetPy)

    ExternalProject_Get_Property(SweetPy INSTALL_DIR)

    set (SweetPy_ROOT_DIR          ${INSTALL_DIR})
    set (SweetPy_INCLUDE_DIR       ${SweetPy_ROOT_DIR}/include)
    set (SweetPy_LIBRARY_DIR       ${SweetPy_ROOT_DIR}/lib)
    set (SweetPy_FOUND             YES)

endif ()
