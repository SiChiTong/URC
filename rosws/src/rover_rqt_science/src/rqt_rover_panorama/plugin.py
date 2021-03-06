#!/usr/bin/env python

from qt_gui.plugin import Plugin
from .plugin_widget import PluginWidget


class PanoramaPlugin(Plugin):
    def __init__(self, context):
        super(PanoramaPlugin, self).__init__(context)
        self.setObjectName('PANOSciencePlugin')
        self._widget = PluginWidget()
        self._widget.setObjectName("PANOSciencePluginUI")

        if context.serial_number() > 1:
            self._widget.setWindowTitle(self._widget.windowTitle() + (' (%d)' % context.serial_number()))

        context.add_widget(self._widget)

    def shutdown_plugin(self):
        pass