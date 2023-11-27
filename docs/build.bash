#!/bin/bash
asciidoctor -r asciidoctor-multipage \
            -b multipage_html5 \
            -D html \
            index.adoc
