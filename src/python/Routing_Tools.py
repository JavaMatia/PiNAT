import subprocess
from time import sleep


def check_interface(interface):
    """
    Returns a valid interface name
    """
    proc = subprocess.Popen(
        ["nmcli", "-f", "DEVICE", "device"], stdout=subprocess.PIPE)
    (available_adapters, _) = proc.communicate()

    # # Does the adapter exist?
    return interface + " " in available_adapters.decode()


def check_password(passphrase):
    """
    Returns a valid WPA2 password
    """
    return 8 <= len(passphrase) <= 63


def check_ssid(ssid):
    """
    Returns a valid SSID
    """
    return 1 < len(ssid) < 32 and '|' not in ssid


def _turn_on(wifi_adapter, etherent_adapter, ssid, password, router_mac):
    """
    Turns hotspot on
    """

    subprocess.Popen(["create_ap", "-m", "bridge", wifi_adapter, etherent_adapter,
                      ssid, password, "--daemon"])

    sleep(8)
    subprocess.Popen(["ebtables", "-A", "FORWARD", "-j", "DROP",
                      "-o", "ap0", "-s", router_mac]).wait()
    subprocess.Popen(["ebtables", "-A", "FORWARD", "-j", "DROP",
                      "-o", etherent_adapter, "-d", router_mac]).wait()

    subprocess.Popen(["ethtool", "-K", "ap0", "gso", "off"]).wait()
    subprocess.Popen(["ethtool", "-K", "ap0", "gro", "off"]).wait()

    subprocess.Popen(["ethtool", "-K", etherent_adapter, "gso", "off"]).wait()
    subprocess.Popen(["ethtool", "-K", etherent_adapter, "gro", "off"]).wait()

    subprocess.Popen(["ip", "link", "set", "dev", etherent_adapter, "mtu",
                      "2000"]).wait()
    subprocess.Popen(["ip", "link", "set", "dev", "ap0", "mtu", "2000"]).wait()

    subprocess.Popen(["ip", "link", "set", "dev",
                      "br0", "promisc", "on"]).wait()

    subprocess.Popen(["ip", "route", "del", "to", "default", "dev", "br0"]).wait()
    print("Hotspot has been activated")


def cleanup(wifi_interface, eth_interface, router_mac):
    """ 
    Turns off the hotspot if neccessry
    """
    subprocess.Popen(["create_ap", "--stop", wifi_interface]).wait()
    subprocess.Popen(["ebtables", "-D", "FORWARD", "-j",
                      "DROP", "-o", "ap0", "-s", router_mac]).wait()
    subprocess.Popen(["ebtables", "-D", "FORWARD", "-j", "DROP",
                      "-o", eth_interface, "-d", router_mac]).wait()

    subprocess.Popen(["ethtool", "-K", "ap0", "gso", "on"]).wait()
    subprocess.Popen(["ethtool", "-K", "ap0", "gro", "on"]).wait()

    subprocess.Popen(["ethtool", "-K", eth_interface, "gso", "on"]).wait()
    subprocess.Popen(["ethtool", "-K", eth_interface, "gro", "on"]).wait()

    subprocess.Popen(["ip", "link", "set", "dev", eth_interface, "mtu",
                      "1500"]).wait()
    subprocess.Popen(["ip", "link", "set", "dev", "ap0", "mtu", "1500"]).wait()

    print("Hotspot deactivated")


def read_config():
    try:
        f = open("config.txt", "r")
    except OSError:
        return None, None, None, None

    hotspot_interface = None
    internet_interface = None
    ssid = None
    password = None
    for line in f.read().split("\n"):
        try:
            key, value = line.split("=")
        except:
            continue
        if key == "hotspot_interface":
            hotspot_interface = value
        elif key == "internet_interface":
            internet_interface = value
        elif key == "ssid":
            ssid = value
        elif key == "pass":
            password = value
    f.close()

    return hotspot_interface, internet_interface, ssid, password


def init_hotspot(router_mac):
    wifi_adapter, eth_adapter, ssid, password = read_config()

    if wifi_adapter == None:
        wifi_adapter = input("Wifi interface name: ")
    else:
        print("wifi_adapter = " + wifi_adapter)
    if not check_interface(wifi_adapter):
        raise Exception("interface {} does not exist".format(wifi_adapter))

    if eth_adapter == None:
        eth_adapter = input("Ethernet interface name: ")
    else:
        print("regular_adapter = " + eth_adapter)
    if not check_interface(eth_adapter):
        raise Exception("interface {} does not exist".format(eth_adapter))

    if ssid == None:
        ssid = input("Wifi name: ")
    else:
        print("ssid = " + ssid)
    if not check_ssid(ssid):
        raise Exception("Invalid ssid: " + ssid)

    if password == None:
        password = input("Wifi password: ")
    else:
        print("password = {}".format(password))
    if not check_password(password):
        raise Exception(
            "Invalid WPA2 password: {}\nPassword must be 8-63 chars long".format(password))

    # Start hotspot:
    _turn_on(wifi_adapter, eth_adapter, ssid, password, router_mac)
    return wifi_adapter, eth_adapter
