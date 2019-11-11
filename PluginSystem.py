# Inspiration: https://github.com/gdiepen/python_plugin_example
import os, importlib, pkgutil

class plugin:
    """All plugins will have to inherit this class and implement 
    its methods"""

    def __init__(self, name, author, version, description, plugin_type):
        """In order to load a plugin, we need to have the following information:
        - name of the plugin
        - author/s of the plugin
        - the current version of the plugin 
        - the description of the plugin - what does it do?
        - the plugin type (maybe in the future this will come in handy)
        It is possible and even recommended to have a config file and load the information from there
        """
        self.name = name
        self.author = author
        self.version = version
        self.description = description
        self.type = plugin_type

    # Now here we can add all the methods the plugin has to have:
    
    def action1(self):
       raise NotImplementedError


class plugin_system:
    """This class will load every plugin (File which inherits from plugin class)"""

    def __init__(self, plugins_location):
        self.plugins_location = plugins_location


    def reload(self):
        """Reset the list of all plugins and initiate the walk over the main
        provided plugin package to load all available plugins
        """
        self.plugins = []
        print(f'Looking for plugins under package {self.plugins_location}')
        for plugin_folder in os.listdir(self.plugins_location):
            self.import_plugin(self.plugins_location + "." + plugin_folder)
        return self.plugins


    def import_plugin(self, plugin_folder):
        """Find the file which inherit plugin and import it
        """
        # Plugin_folder - the folder containing the
        try:
            plugin_module = importlib.import_module(plugin_folder+"."+plugin_folder[plugin_folder.index('.') + 1:])
            plugin_class = plugin_module.__getattribute__(plugin_folder[plugin_folder.index('.') + 1:])
            # Check if the module is not subclass of plugin or is plugin
            if not issubclass(plugin_class, plugin) or plugin_class is plugin:
                del plugin_module
                raise Exception(f"The plugin {plugin_class} is not a child of plugin or is plugin itself")
        except ImportError as e:
            print(e)
        except Exception as e:
            print(e)
        self.plugins.append(plugin_class)
        