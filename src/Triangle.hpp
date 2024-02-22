#define GLFW_INCLUDE_VULKAN
#define WIDTH 1280
#define HEIGHT 720

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

class Triangle {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    };

private:
    GLFWwindow* window = nullptr;
    VkInstance instance;

    void initWindow() {
        if (!glfwInit()) {
            glfwTerminate();
            throw new std::exception();
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // dont create opengl context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);    // disable resize
        window = glfwCreateWindow(640, 480, "My Title", nullptr, nullptr);

        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
    };

    void initVulkan() {
        createInstance();
    };

    void createInstance() {
        VkApplicationInfo appInfo{};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // specifying the global extensions to interface with the OS
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        // we need this to enable portability things for MoltenVK on Macos
        std::vector<const char*> requiredExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }

        // portability things for macos
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        createInfo.enabledLayerCount = 0;  // empty for now, no validation layers

        // nullptr is the pointer to custom allocator callback
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("faiiled to create vulkan instance");
        }
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();
    }
};