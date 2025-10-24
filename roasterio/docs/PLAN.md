# Smart Home Coffee Roaster Project Plan
## From Emerio POM-111664.8 to Commercial Product

---

## 🎯 Project Vision

Transform the Emerio POM-111664.8 popcorn maker into an intelligent, AI-powered coffee roaster that surpasses current home and commercial roasters through machine learning, precise sensor control, and automated profile optimization using **ROS2 (Robot Operating System 2)** as the core framework. Ultimate goal: A commercializable product with an online shop selling both roasters and curated green beans with scan-to-roast technology.

**Inspiration:** Gaggiuino project (espresso machine modding) applied to coffee roasting, enhanced with professional robotics architecture

**Why ROS2:** Provides distributed computing, real-time control, advanced data management, and professional-grade tooling that transforms this from a hobby project into a production-ready system.

---

## 📊 Project Phases Overview

| Phase | Focus                    | Duration    | Investment |
| ----- | ------------------------ | ----------- | ---------- |
| 0     | Research & Setup         | 2-3 weeks   | €200-300   |
| 1     | Basic Functional Roaster | 2-4 weeks   | €100-200   |
| 2     | Smart Control System     | 4-6 weeks   | €200-400   |
| 3     | Advanced Sensing & UI    | 6-8 weeks   | €300-500   |
| 4     | AI & Machine Learning    | 8-12 weeks  | €200-300   |
| 5     | Refinement & Safety      | 4-6 weeks   | €300-500   |
| 6     | Prototype Finalization   | 6-8 weeks   | €500-800   |
| 7     | Commercialization        | 6-12 months | Variable   |

**Total Estimated Timeline:** 8-12 months for working prototype, 18-24 months to market

---

## 🔬 Phase 0: Research & Preparation

### Objectives
- Understand coffee roasting fundamentals
- Learn from existing DIY roaster community
- Acquire initial tools and materials
- Set up development environment

### Tasks
1. **Coffee Roasting Education**
   - Study roast profiles (light, medium, dark)
   - Learn about first crack, second crack, and development time
   - Understand Rate of Rise (RoR) and Bean Temperature (BT) curves
   - Research different origins and their roasting characteristics

2. **Technical Research**
   - Join home roasting forums (Home-Roasters.org, r/roasting)
   - Study existing popcorn maker mods (West Bend Poppery, Presto)
   - Analyze Gaggiuino project documentation
   - Review Artisan roasting software

3. **Emerio POM-111664.8 Analysis**
   - Disassemble unit carefully (document with photos)
   - Identify heating element specifications (~1200W)
   - Map electrical connections
   - Understand fan motor type and control
   - Measure roasting chamber dimensions

3. **Development Environment Setup**
   - Install ROS2 Humble/Jazzy on Raspberry Pi 4
   - Set up ROS2 workspace with colcon build system
   - Install Artisan Roaster Scope software
   - Configure micro-ROS for ESP32 safety controller
   - Set up Git repository with ROS2 package structure
   - Install RViz2 and Gazebo for visualization/simulation
   - Create initial ROS2 package structure for roaster control

4. **ROS2 Package Structure Setup**
   ```
   roaster_ws/
   ├── src/
   │   ├── roaster_hardware/          # Hardware interfaces
   │   ├── roaster_control/           # Control algorithms  
   │   ├── roaster_sensors/           # Sensor drivers
   │   ├── roaster_safety/            # Safety monitoring
   │   ├── roaster_ml/                # Machine learning nodes
   │   ├── roaster_web/               # Web interface bridge
   │   ├── roaster_profiles/          # Profile management
   │   └── roaster_msgs/              # Custom message definitions
   ```

### Resources Needed
- Basic electronics toolkit
- Multimeter  
- Soldering station
- Safety equipment (fire extinguisher, heat-resistant gloves)
- Green coffee beans for testing (500g-1kg)
- **Raspberry Pi 4B (4GB+)** for ROS2 development
- MicroSD card (32GB+) with Ubuntu 22.04/24.04
- **ESP32 DevKit** for safety controller (micro-ROS)

### Budget: €200-300

---

## ⚡ Phase 1: Basic Functional Roaster

### Objectives
- Transform popcorn maker into working coffee roaster
- Achieve consistent, repeatable roasts
- Establish baseline performance

### Core Modifications

#### 1. Bypass Safety Thermostat
**Why:** Stock thermostat limits temperature to ~180°C; coffee needs 200-230°C

**Steps:**
- Locate thermostat in heating circuit
- Carefully bypass with direct wire connection
- **SAFETY:** This removes overheating protection - never leave roaster unattended
- Consider adding manual temperature monitoring

#### 2. Separate Fan and Heater Control
**Why:** Independent control allows precise temperature management

**Steps:**
- Identify fan motor circuit
- Separate heater coil circuit
- Create two power inputs with separate switches
- This allows cooling beans with fan after roasting

#### 3. Add Chimney Extension
**Why:** Prevents beans from ejecting during roast

**Materials:**
- Aluminum flashing or soup can
- Heat-resistant construction
- 10-15cm height extension
- Optional: integrated chaff collector

#### 4. Temperature Monitoring (Manual)
**Materials:**
- K-type thermocouple (up to 400°C)
- Handheld thermometer with K-type input
- Position probe ~1cm above bean bed

### Testing Protocol
1. Start with 60-80g green beans
2. Preheat empty for 1 minute
3. Add beans, monitor temperature
4. Target temperature progression:
   - 0-3 min: 150-170°C (drying phase)
   - 3-8 min: 170-205°C (Maillard reactions)
   - First crack: ~205°C
   - 8-12 min: 205-225°C (development)
   - Second crack: ~225°C (if desired)

### Success Criteria
- Consistently reach 220°C+
- Complete roast in 8-15 minutes
- Even roast color across batch
- Minimal scorching or tipping
- Repeatable results

### Budget: €100-200

---

## 🤖 Phase 2: Smart Control System

### Objectives
- Add Arduino-based control
- Implement basic temperature control
- Begin data logging
- Create foundation for automation

### Hardware Components

#### Core Controller
**Options:**
1. **Raspberry Pi 4B** (€70-90) **[RECOMMENDED for ROS2]**
   - Full Linux system for ROS2
   - Excellent processing power
   - Built-in WiFi/Bluetooth/Ethernet
   - GPIO for hardware control
   - 4GB+ RAM recommended
   
2. **Raspberry Pi CM4** (€40-70)
   - Compact compute module
   - Custom carrier board design
   - Industrial applications
   - Better for final product
   
3. **ESP32** (€8-15) **[SECONDARY/MICROCONTROLLER]**
   - Real-time hardware control
   - Communicate with ROS2 via micro-ROS
   - Handle safety-critical functions
   - Low-level sensor interfacing

#### Temperature Sensing
- **MAX31855 or MAX6675 Thermocouple Amplifier** (€8-12)
- **K-Type Thermocouple** (€5-15)
  - Bean temperature probe (primary)
  - Consider adding: exhaust temp, ambient temp
  
#### Power Control
- **40DA Solid State Relay (SSR)** (€8-15)
  - Controls heater on/off
  - Zero-crossing for smooth operation
  - Heat sink required
  
- **Dimmer Module** (€10-20)
  - PWM control for fan speed
  - AC dimmer (TRIAC-based)
  
#### Interface (Initial)
- **Simple LCD Display** (€5-10)
  - 16x2 or 20x4 character display
  - Shows: BT, Time, Power%
  
- **Rotary Encoder** (€3-5)
  - Manual power adjustment
  - Menu navigation

### Wiring Diagram
```
[Mains 230V] ──┬──> [SSR] ──> [Heating Element]
               │
               └──> [Dimmer] ──> [Fan Motor]

[Raspberry Pi 4] ──> [GPIO Expander] ──> [SSR Control]
                 │                    ──> [Dimmer Control]
                 │
                 ├──> [SPI] ──> [MAX31855] ──> [K-Type Thermocouple]
                 ├──> [I2C] ──> [BME280] (Environmental)
                 ├──> [I2S] ──> [MEMS Microphone]
                 └──> [USB] ──> [ESP32] (Safety Controller)
                          └──> [Emergency Stop Hardware]
```

### Software Features (Initial)

#### 1. ROS2 Architecture
```cpp
// ROS2 Node Structure
roast_controller/
├── temperature_node      // Temperature sensing & control
├── power_control_node   // Heater/fan control
├── safety_monitor_node  // Safety systems
├── data_logger_node     // Data collection
├── web_interface_node   // Web UI bridge
└── profile_manager_node // Profile execution

// Key ROS2 Features:
- Real-time control via DDS
- Distributed computing
- Parameter server for configuration
- Action servers for roast profiles
- Service calls for manual control
- Quality of Service (QoS) for reliability
```

#### 2. Temperature Control with ROS2
```python
# Python ROS2 node example
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Temperature
from control_msgs.action import FollowJointTrajectory

class TemperatureController(Node):
    def __init__(self):
        super().__init__('temperature_controller')
        
        # Publishers
        self.heater_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # Subscribers  
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temperature_callback, 10)
            
        # Parameters
        self.declare_parameter('target_temp', 200.0)
        self.declare_parameter('pid_kp', 2.0)
        
        # Timer for control loop
        self.timer = self.create_timer(0.1, self.control_loop)
        
    def control_loop(self):
        # PID control implementation
        power = self.compute_pid()
        self.publish_heater_power(power)
```

#### 3. Data Logging with ROS2
- **ROS2 Bag Files:** Record all sensor data, commands, and states
- **Structured Topics:** Separate topics for each sensor type
- **Time Synchronization:** Accurate timestamps for all data
- **Replay Capability:** Re-run roasts for analysis and ML training

#### 4. Profile Execution
```python
# ROS2 Action Server for roast profiles
class RoastProfileAction(Node):
    def __init__(self):
        super().__init__('roast_profile_executor')
        
        self._action_server = ActionServer(
            self, RoastProfile, 'execute_roast',
            self.execute_callback)
    
    def execute_callback(self, goal_handle):
        # Load profile from goal
        profile = goal_handle.request.profile
        
        # Execute profile with feedback
        for phase in profile.phases:
            self.execute_phase(phase)
            
            # Send feedback
            feedback = RoastProfile.Feedback()
            feedback.current_phase = phase.name
            feedback.progress = self.calculate_progress()
            goal_handle.publish_feedback(feedback)
        
        goal_handle.succeed()
        return RoastProfile.Result()
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror: 8.0
      - phase: "development"
        duration: 180
        target_temp: 210.0
        target_ror: 3.0
    notes: "Drop at first crack + 60s"

# ROS2 Bag Structure for Roast Logs
# Recorded topics:
# - /bean_temperature (sensor_msgs/Temperature)
# - /exhaust_temperature (sensor_msgs/Temperature)
# - /heater_power (std_msgs/Float32)
# - /fan_speed (std_msgs/Float32)
# - /roast_events (custom_msgs/RoastEvent)
# - /crack_detection (custom_msgs/CrackEvent)
# - /safety_status (custom_msgs/SafetyStatus)

# Custom ROS2 Message Types:
roast_metadata:
  id: "uuid"
  timestamp: "2025-01-15T10:30:00Z"
  profile_name: "colombian_light"
  green_weight: 100.0
  roasted_weight: 85.0
  duration: 720
  first_crack_time: 540
  second_crack_time: null
  rating: 4
  notes: "Slightly underdeveloped"
  bag_file_path: "/roast_data/20250115_103000.bag"
```

### Control Algorithms

#### PID Temperature Control with ROS2
ROS2 control framework implementation:
```python
# ROS2 Control Hardware Interface
class RoasterHardwareInterface(HardwareInterface):
    def __init__(self):
        super().__init__()
        
        # Define joints (controllable outputs)
        self.heater_power = 0.0
        self.fan_speed = 0.0
        
        # Define sensors (readable inputs)  
        self.bean_temp = 0.0
        self.exhaust_temp = 0.0
        
    def read(self, time, period):
        # Read sensor values
        self.bean_temp = self.read_thermocouple()
        self.exhaust_temp = self.read_exhaust_sensor()
        
    def write(self, time, period):
        # Write control values
        self.set_heater_power(self.heater_power)
        self.set_fan_speed(self.fan_speed)

# ROS2 Control Configuration (YAML)
controller_manager:
  ros__parameters:
    update_rate: 100  # 100 Hz control loop
    
roaster_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints:
    - heater_power
    - fan_speed
    
temperature_pid:
  type: pid_controller/PidController
  reference_and_state_interfaces:
    - bean_temperature
  command_interfaces:
    - heater_power
  pid:
    p: 2.0
    i: 0.5
    d: 1.0
```

#### Rate of Rise (RoR) Control with ROS2
ROS2 trajectory controller for RoR targets:
```python
# Generate trajectory for desired RoR profile
class RoRTrajectoryGenerator(Node):
    def __init__(self):
        super().__init__('ror_trajectory_generator')
        
        self.trajectory_pub = self.create_publisher(
            JointTrajectory, 
            '/roaster_controller/joint_trajectory', 10)
    
    def generate_ror_trajectory(self, target_ror_profile):
        trajectory = JointTrajectory()
        trajectory.joint_names = ['bean_temperature']
        
        for i, ror_point in enumerate(target_ror_profile):
            point = JointTrajectoryPoint()
            point.positions = [ror_point.temperature]
            point.velocities = [ror_point.ror / 60.0]  # RoR in °C/s
            point.time_from_start = Duration(seconds=i)
            trajectory.points.append(point)
        
        self.trajectory_pub.publish(trajectory)
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror: 8.0
      - phase: "development"
        duration: 180
        target_temp: 210.0
        target_ror: 3.0
    notes: "Drop at first crack + 60s"

# ROS2 Bag Structure for Roast Logs
# Recorded topics:
# - /bean_temperature (sensor_msgs/Temperature)
# - /exhaust_temperature (sensor_msgs/Temperature)
# - /heater_power (std_msgs/Float32)
# - /fan_speed (std_msgs/Float32)
# - /roast_events (custom_msgs/RoastEvent)
# - /crack_detection (custom_msgs/CrackEvent)
# - /safety_status (custom_msgs/SafetyStatus)

# Custom ROS2 Message Types:
roast_metadata:
  id: "uuid"
  timestamp: "2025-01-15T10:30:00Z"
  profile_name: "colombian_light"
  green_weight: 100.0
  roasted_weight: 85.0
  duration: 720
  first_crack_time: 540
  second_crack_time: null
  rating: 4
  notes: "Slightly underdeveloped"
  bag_file_path: "/roast_data/20250115_103000.bag"
```

### Control Algorithms

#### PID Temperature Control with ROS2
ROS2 control framework implementation:
```python
# ROS2 Control Hardware Interface
class RoasterHardwareInterface(HardwareInterface):
    def __init__(self):
        super().__init__()
        
        # Define joints (controllable outputs)
        self.heater_power = 0.0
        self.fan_speed = 0.0
        
        # Define sensors (readable inputs)  
        self.bean_temp = 0.0
        self.exhaust_temp = 0.0
        
    def read(self, time, period):
        # Read sensor values
        self.bean_temp = self.read_thermocouple()
        self.exhaust_temp = self.read_exhaust_sensor()
        
    def write(self, time, period):
        # Write control values
        self.set_heater_power(self.heater_power)
        self.set_fan_speed(self.fan_speed)

# ROS2 Control Configuration (YAML)
controller_manager:
  ros__parameters:
    update_rate: 100  # 100 Hz control loop
    
roaster_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints:
    - heater_power
    - fan_speed
    
temperature_pid:
  type: pid_controller/PidController
  reference_and_state_interfaces:
    - bean_temperature
  command_interfaces:
    - heater_power
  pid:
    p: 2.0
    i: 0.5
    d: 1.0
```

#### Rate of Rise (RoR) Control with ROS2
ROS2 trajectory controller for RoR targets:
```python
# Generate trajectory for desired RoR profile
class RoRTrajectoryGenerator(Node):
    def __init__(self):
        super().__init__('ror_trajectory_generator')
        
        self.trajectory_pub = self.create_publisher(
            JointTrajectory, 
            '/roaster_controller/joint_trajectory', 10)
    
    def generate_ror_trajectory(self, target_ror_profile):
        trajectory = JointTrajectory()
        trajectory.joint_names = ['bean_temperature']
        
        for i, ror_point in enumerate(target_ror_profile):
            point = JointTrajectoryPoint()
            point.positions = [ror_point.temperature]
            point.velocities = [ror_point.ror / 60.0]  # RoR in °C/s
            point.time_from_start = Duration(seconds=i)
            trajectory.points.append(point)
        
        self.trajectory_pub.publish(trajectory)
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror: 8.0
      - phase: "development"
        duration: 180
        target_temp: 210.0
        target_ror: 3.0
    notes: "Drop at first crack + 60s"

# ROS2 Bag Structure for Roast Logs
# Recorded topics:
# - /bean_temperature (sensor_msgs/Temperature)
# - /exhaust_temperature (sensor_msgs/Temperature)
# - /heater_power (std_msgs/Float32)
# - /fan_speed (std_msgs/Float32)
# - /roast_events (custom_msgs/RoastEvent)
# - /crack_detection (custom_msgs/CrackEvent)
# - /safety_status (custom_msgs/SafetyStatus)

# Custom ROS2 Message Types:
roast_metadata:
  id: "uuid"
  timestamp: "2025-01-15T10:30:00Z"
  profile_name: "colombian_light"
  green_weight: 100.0
  roasted_weight: 85.0
  duration: 720
  first_crack_time: 540
  second_crack_time: null
  rating: 4
  notes: "Slightly underdeveloped"
  bag_file_path: "/roast_data/20250115_103000.bag"
```

### Control Algorithms

#### PID Temperature Control with ROS2
ROS2 control framework implementation:
```python
# ROS2 Control Hardware Interface
class RoasterHardwareInterface(HardwareInterface):
    def __init__(self):
        super().__init__()
        
        # Define joints (controllable outputs)
        self.heater_power = 0.0
        self.fan_speed = 0.0
        
        # Define sensors (readable inputs)  
        self.bean_temp = 0.0
        self.exhaust_temp = 0.0
        
    def read(self, time, period):
        # Read sensor values
        self.bean_temp = self.read_thermocouple()
        self.exhaust_temp = self.read_exhaust_sensor()
        
    def write(self, time, period):
        # Write control values
        self.set_heater_power(self.heater_power)
        self.set_fan_speed(self.fan_speed)

# ROS2 Control Configuration (YAML)
controller_manager:
  ros__parameters:
    update_rate: 100  # 100 Hz control loop
    
roaster_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints:
    - heater_power
    - fan_speed
    
temperature_pid:
  type: pid_controller/PidController
  reference_and_state_interfaces:
    - bean_temperature
  command_interfaces:
    - heater_power
  pid:
    p: 2.0
    i: 0.5
    d: 1.0
```

#### Rate of Rise (RoR) Control with ROS2
ROS2 trajectory controller for RoR targets:
```python
# Generate trajectory for desired RoR profile
class RoRTrajectoryGenerator(Node):
    def __init__(self):
        super().__init__('ror_trajectory_generator')
        
        self.trajectory_pub = self.create_publisher(
            JointTrajectory, 
            '/roaster_controller/joint_trajectory', 10)
    
    def generate_ror_trajectory(self, target_ror_profile):
        trajectory = JointTrajectory()
        trajectory.joint_names = ['bean_temperature']
        
        for i, ror_point in enumerate(target_ror_profile):
            point = JointTrajectoryPoint()
            point.positions = [ror_point.temperature]
            point.velocities = [ror_point.ror / 60.0]  # RoR in °C/s
            point.time_from_start = Duration(seconds=i)
            trajectory.points.append(point)
        
        self.trajectory_pub.publish(trajectory)
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror: 8.0
      - phase: "development"
        duration: 180
        target_temp: 210.0
        target_ror: 3.0
    notes: "Drop at first crack + 60s"

# ROS2 Bag Structure for Roast Logs
# Recorded topics:
# - /bean_temperature (sensor_msgs/Temperature)
# - /exhaust_temperature (sensor_msgs/Temperature)
# - /heater_power (std_msgs/Float32)
# - /fan_speed (std_msgs/Float32)
# - /roast_events (custom_msgs/RoastEvent)
# - /crack_detection (custom_msgs/CrackEvent)
# - /safety_status (custom_msgs/SafetyStatus)

# Custom ROS2 Message Types:
roast_metadata:
  id: "uuid"
  timestamp: "2025-01-15T10:30:00Z"
  profile_name: "colombian_light"
  green_weight: 100.0
  roasted_weight: 85.0
  duration: 720
  first_crack_time: 540
  second_crack_time: null
  rating: 4
  notes: "Slightly underdeveloped"
  bag_file_path: "/roast_data/20250115_103000.bag"
```

### Control Algorithms

#### PID Temperature Control with ROS2
ROS2 control framework implementation:
```python
# ROS2 Control Hardware Interface
class RoasterHardwareInterface(HardwareInterface):
    def __init__(self):
        super().__init__()
        
        # Define joints (controllable outputs)
        self.heater_power = 0.0
        self.fan_speed = 0.0
        
        # Define sensors (readable inputs)  
        self.bean_temp = 0.0
        self.exhaust_temp = 0.0
        
    def read(self, time, period):
        # Read sensor values
        self.bean_temp = self.read_thermocouple()
        self.exhaust_temp = self.read_exhaust_sensor()
        
    def write(self, time, period):
        # Write control values
        self.set_heater_power(self.heater_power)
        self.set_fan_speed(self.fan_speed)

# ROS2 Control Configuration (YAML)
controller_manager:
  ros__parameters:
    update_rate: 100  # 100 Hz control loop
    
roaster_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints:
    - heater_power
    - fan_speed
    
temperature_pid:
  type: pid_controller/PidController
  reference_and_state_interfaces:
    - bean_temperature
  command_interfaces:
    - heater_power
  pid:
    p: 2.0
    i: 0.5
    d: 1.0
```

#### Rate of Rise (RoR) Control with ROS2
ROS2 trajectory controller for RoR targets:
```python
# Generate trajectory for desired RoR profile
class RoRTrajectoryGenerator(Node):
    def __init__(self):
        super().__init__('ror_trajectory_generator')
        
        self.trajectory_pub = self.create_publisher(
            JointTrajectory, 
            '/roaster_controller/joint_trajectory', 10)
    
    def generate_ror_trajectory(self, target_ror_profile):
        trajectory = JointTrajectory()
        trajectory.joint_names = ['bean_temperature']
        
        for i, ror_point in enumerate(target_ror_profile):
            point = JointTrajectoryPoint()
            point.positions = [ror_point.temperature]
            point.velocities = [ror_point.ror / 60.0]  # RoR in °C/s
            point.time_from_start = Duration(seconds=i)
            trajectory.points.append(point)
        
        self.trajectory_pub.publish(trajectory)
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror: 8.0
      - phase: "development"
        duration: 180
        target_temp: 210.0
        target_ror: 3.0
    notes: "Drop at first crack + 60s"

# ROS2 Bag Structure for Roast Logs
# Recorded topics:
# - /bean_temperature (sensor_msgs/Temperature)
# - /exhaust_temperature (sensor_msgs/Temperature)
# - /heater_power (std_msgs/Float32)
# - /fan_speed (std_msgs/Float32)
# - /roast_events (custom_msgs/RoastEvent)
# - /crack_detection (custom_msgs/CrackEvent)
# - /safety_status (custom_msgs/SafetyStatus)

# Custom ROS2 Message Types:
roast_metadata:
  id: "uuid"
  timestamp: "2025-01-15T10:30:00Z"
  profile_name: "colombian_light"
  green_weight: 100.0
  roasted_weight: 85.0
  duration: 720
  first_crack_time: 540
  second_crack_time: null
  rating: 4
  notes: "Slightly underdeveloped"
  bag_file_path: "/roast_data/20250115_103000.bag"
```

### Control Algorithms

#### PID Temperature Control with ROS2
ROS2 control framework implementation:
```python
# ROS2 Control Hardware Interface
class RoasterHardwareInterface(HardwareInterface):
    def __init__(self):
        super().__init__()
        
        # Define joints (controllable outputs)
        self.heater_power = 0.0
        self.fan_speed = 0.0
        
        # Define sensors (readable inputs)  
        self.bean_temp = 0.0
        self.exhaust_temp = 0.0
        
    def read(self, time, period):
        # Read sensor values
        self.bean_temp = self.read_thermocouple()
        self.exhaust_temp = self.read_exhaust_sensor()
        
    def write(self, time, period):
        # Write control values
        self.set_heater_power(self.heater_power)
        self.set_fan_speed(self.fan_speed)

# ROS2 Control Configuration (YAML)
controller_manager:
  ros__parameters:
    update_rate: 100  # 100 Hz control loop
    
roaster_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints:
    - heater_power
    - fan_speed
    
temperature_pid:
  type: pid_controller/PidController
  reference_and_state_interfaces:
    - bean_temperature
  command_interfaces:
    - heater_power
  pid:
    p: 2.0
    i: 0.5
    d: 1.0
```

#### Rate of Rise (RoR) Control with ROS2
ROS2 trajectory controller for RoR targets:
```python
# Generate trajectory for desired RoR profile
class RoRTrajectoryGenerator(Node):
    def __init__(self):
        super().__init__('ror_trajectory_generator')
        
        self.trajectory_pub = self.create_publisher(
            JointTrajectory, 
            '/roaster_controller/joint_trajectory', 10)
    
    def generate_ror_trajectory(self, target_ror_profile):
        trajectory = JointTrajectory()
        trajectory.joint_names = ['bean_temperature']
        
        for i, ror_point in enumerate(target_ror_profile):
            point = JointTrajectoryPoint()
            point.positions = [ror_point.temperature]
            point.velocities = [ror_point.ror / 60.0]  # RoR in °C/s
            point.time_from_start = Duration(seconds=i)
            trajectory.points.append(point)
        
        self.trajectory_pub.publish(trajectory)
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror: 8.0
      - phase: "development"
        duration: 180
        target_temp: 210.0
        target_ror: 3.0
    notes: "Drop at first crack + 60s"

# ROS2 Bag Structure for Roast Logs
# Recorded topics:
# - /bean_temperature (sensor_msgs/Temperature)
# - /exhaust_temperature (sensor_msgs/Temperature)
# - /heater_power (std_msgs/Float32)
# - /fan_speed (std_msgs/Float32)
# - /roast_events (custom_msgs/RoastEvent)
# - /crack_detection (custom_msgs/CrackEvent)
# - /safety_status (custom_msgs/SafetyStatus)

# Custom ROS2 Message Types:
roast_metadata:
  id: "uuid"
  timestamp: "2025-01-15T10:30:00Z"
  profile_name: "colombian_light"
  green_weight: 100.0
  roasted_weight: 85.0
  duration: 720
  first_crack_time: 540
  second_crack_time: null
  rating: 4
  notes: "Slightly underdeveloped"
  bag_file_path: "/roast_data/20250115_103000.bag"
```

### Control Algorithms

#### PID Temperature Control with ROS2
ROS2 control framework implementation:
```python
# ROS2 Control Hardware Interface
class RoasterHardwareInterface(HardwareInterface):
    def __init__(self):
        super().__init__()
        
        # Define joints (controllable outputs)
        self.heater_power = 0.0
        self.fan_speed = 0.0
        
        # Define sensors (readable inputs)  
        self.bean_temp = 0.0
        self.exhaust_temp = 0.0
        
    def read(self, time, period):
        # Read sensor values
        self.bean_temp = self.read_thermocouple()
        self.exhaust_temp = self.read_exhaust_sensor()
        
    def write(self, time, period):
        # Write control values
        self.set_heater_power(self.heater_power)
        self.set_fan_speed(self.fan_speed)

# ROS2 Control Configuration (YAML)
controller_manager:
  ros__parameters:
    update_rate: 100  # 100 Hz control loop
    
roaster_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints:
    - heater_power
    - fan_speed
    
temperature_pid:
  type: pid_controller/PidController
  reference_and_state_interfaces:
    - bean_temperature
  command_interfaces:
    - heater_power
  pid:
    p: 2.0
    i: 0.5
    d: 1.0
```

#### Rate of Rise (RoR) Control with ROS2
ROS2 trajectory controller for RoR targets:
```python
# Generate trajectory for desired RoR profile
class RoRTrajectoryGenerator(Node):
    def __init__(self):
        super().__init__('ror_trajectory_generator')
        
        self.trajectory_pub = self.create_publisher(
            JointTrajectory, 
            '/roaster_controller/joint_trajectory', 10)
    
    def generate_ror_trajectory(self, target_ror_profile):
        trajectory = JointTrajectory()
        trajectory.joint_names = ['bean_temperature']
        
        for i, ror_point in enumerate(target_ror_profile):
            point = JointTrajectoryPoint()
            point.positions = [ror_point.temperature]
            point.velocities = [ror_point.ror / 60.0]  # RoR in °C/s
            point.time_from_start = Duration(seconds=i)
            trajectory.points.append(point)
        
        self.trajectory_pub.publish(trajectory)
```

### Integration with Artisan & ROS2
- **ROS2 Bridge:** Custom node to interface with Artisan software
- **Real-time Communication:** WebSocket bridge between ROS2 and Artisan
- **Data Exchange:** Temperature readings, control commands, profile data
- **Visualization:** RViz for system monitoring and debugging
- **Command Line Tools:** `ros2 topic` commands for manual control

```bash
# Example ROS2 commands for roaster control
ros2 topic pub /heater_power std_msgs/Float32 "data: 75.0"
ros2 topic echo /bean_temperature
ros2 service call /start_roast roaster_interfaces/StartRoast "{profile_name: 'colombian_light'}"
ros2 bag record -a  # Record complete roast session
```

### Safety Features
- Maximum temperature limit (240°C cutoff) via ROS2 parameter server
- Timeout protection (auto-shutoff) with ROS2 timers
- Emergency stop service: `ros2 service call /emergency_stop`
- Hardware watchdog integration with ROS2 health monitoring
- Distributed safety: Multiple nodes can trigger emergency stop
- Safety state machine published to `/safety_status` topic

### Budget: €200-400

---

## 📡 Phase 3: Advanced Sensing & Web UI

### Objectives
- Comprehensive sensor array
- Professional web interface
- Real-time monitoring and control
- Profile creation and management

### Enhanced Hardware

#### Additional Sensors

1. **Bean Temperature (BT)**
   - Primary K-type thermocouple
   - Positioned in bean mass
   
2. **Exhaust Temperature (ET)**
   - Secondary K-type thermocouple
   - Positioned in exhaust stream
   - Indicates heat transfer efficiency
   
3. **Environmental Temperature (ET)**
   - BME280 or DHT22 sensor (€5-15)
   - Ambient temp, humidity
   - Affects roast profiles
   
4. **Pressure/Airflow Sensor** (Optional)
   - Differential pressure sensor (€15-30)
   - Monitor fan performance
   - Detect chaff buildup
   
5. **Weight Scale** (Future consideration)
   - HX711 load cell amplifier + load cell (€15-30)
   - Real-time weight monitoring
   - Calculate moisture loss
   
6. **Acoustic Sensor**
   - Electret microphone + amplifier (€5-10)
   - First/second crack detection
   - Critical for ML training later

#### Display Upgrade
Replace LCD with:
- **Nextion Touch Display** (2.4" - 7") (€20-60)
  - Touch interface like Gaggiuino
  - Custom GUI design
  - Visual roast curve display
  
OR
- **Keep simple display, focus on Web UI**

### Web Interface Development

### Web Interface Development

#### Backend (ROS2 + Web Bridge)
```python
# ROS2 Web Bridge Node
class WebBridgeNode(Node):
    def __init__(self):
        super().__init__('web_bridge')
        
        # Create web server (FastAPI/Flask)
        self.app = FastAPI()
        
        # ROS2 subscribers for data
        self.temp_sub = self.create_subscription(
            Temperature, 'bean_temperature', 
            self.temp_callback, 10)
        
        # ROS2 publishers for control
        self.power_pub = self.create_publisher(
            Float32, 'heater_power', 10)
        
        # WebSocket for real-time data
        @self.app.websocket("/ws")
        async def websocket_endpoint(websocket: WebSocket):
            await self.handle_websocket(websocket)
        
        # REST API endpoints
        @self.app.post("/api/start_roast")
        async def start_roast(profile: RoastProfile):
            return await self.call_roast_service(profile)

# Key features:
- ROS2 parameter server integration
- Real-time data streaming via WebSocket
- REST API for control commands
- Profile storage in ROS2 parameter files
- Integration with ros2_web_bridge package
```

#### Frontend (React/Vue.js with ROS2 Integration)
**Features:**
1. **Dashboard**
   - Live temperature graph (subscribes to ROS2 topics)
   - Current roast progress from action server feedback
   - Time remaining estimate from ROS2 calculations
   - Power/fan controls via ROS2 services
   
2. **Profile Management**
   - Create/edit/delete profiles (stored as ROS2 parameters)
   - Visual curve editor with ROS2 trajectory generation
   - Profile library (light/medium/dark presets)
   - Import/export profiles (ROS2 bag format + JSON)
   
3. **Roast History**
   - Past roasts from ROS2 bag files
   - Comparison tools using rosbag analysis
   - Notes and rating system (custom ROS2 messages)
   - Photos of roasted beans with metadata
   
4. **Manual Control**
   - Power slider (publishes to `/heater_power` topic)
   - Fan speed control (publishes to `/fan_speed` topic)  
   - Emergency stop (calls `/emergency_stop` service)
   - Cooling cycle trigger (calls `/start_cooling` service)
   
5. **ROS2 System Monitor**
   - Node status and health monitoring
   - Topic monitoring and message rates
   - Parameter server browser
   - System diagnostics from `/diagnostics` topic
   
6. **Settings**
   - ROS2 parameter configuration
   - Network settings (DDS configuration)
   - Sensor calibration via parameter updates
   - Safety limits (ROS2 parameter server)
   - Firmware updates (systemd service management)

### Database Structure
```yaml
# ROS2 Parameter Structure (YAML)
roast_profiles:
  colombian_light:
    name: "Colombian Light"
    origin: "Colombia Huila"  
    target_weight: 100
    phases:
      - phase: "preheat"
        duration: 60
        target_temp: 150.0
        target_ror: 0.0
      - phase: "dry"
        duration: 240
        target_temp: 160.0
        target_ror: 15.0
      - phase: "maillard" 
        duration: 360
        target_temp: 195.0
        target_ror