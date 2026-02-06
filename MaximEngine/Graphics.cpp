#include "CookbookSampleFramework.h"
#include "Graphics.h"

using namespace VulkanCookbook;

class Graphics : public VulkanCookbook::VulkanCookbookSample
{

std::vector<VkBuffer>                   vectorVertexBuffer;
std::vector<VkDeviceMemory>             vectorVertexBufferMemory;

VkDestroyer(VkBuffer)                   VertexBuffer2;
VkDestroyer(VkDeviceMemory)             VertexBufferMemory2;

VkDestroyer(VkImage)                    HeightMap;
VkDestroyer(VkDeviceMemory)             HeightMapMemory;
VkDestroyer(VkImageView)                HeightMapView;
VkDestroyer(VkSampler)                  HeightSampler;

VkDestroyer(VkPipelineLayout)           PipelineLayout;
VkDestroyer(VkPipeline)                 Pipeline;

VkDestroyer(VkPipelineLayout)           PipelineLayout2;
VkDestroyer(VkPipeline)                 Pipeline2;

uint32_t                                CurrentPipeline;

VkDestroyer(VkDescriptorSetLayout)      DescriptorSetLayout;
VkDestroyer(VkDescriptorPool)           DescriptorPool;
std::vector<VkDescriptorSet>            DescriptorSets;

VkDestroyer(VkDescriptorSetLayout)  DescriptorSetLayout2;
VkDestroyer(VkDescriptorPool)       DescriptorPool2;
std::vector<VkDescriptorSet>        DescriptorSets2;


bool                                UpdateUniformBuffer;


std::vector<Mesh>                   Model;

VkDestroyer(VkImage)                Image;
VkDestroyer(VkDeviceMemory)         ImageMemory;
VkDestroyer(VkImageView)            ImageView;
VkDestroyer(VkSampler)              Sampler;

VkDestroyer(VkImage)                Image2;
VkDestroyer(VkDeviceMemory)         ImageMemory2;
VkDestroyer(VkImageView)            ImageView2;
VkDestroyer(VkSampler)              Sampler2;

///////////////////////////////////////////////////////////////

Map                                 board;

int                                 meshPos;

///////////////////////////////////////////////////////////////

OrbitingCamera                      Camera;

float rotMax;

public:

    



    Graphics()
    {
        //ModelClass max = ModelClass("MaximZepplin");
        //m_scene = new Scene();
    }

    Graphics(Scene scene)
    {
        //m_scene = &scene;
    }

public:

    bool Initialize(WindowParameters window_parameters)
    {     

      UpdateUniformBuffer = true;

      

      if( !InitializeVulkan( window_parameters ) ) {
        return false;
      }
      
      board.Initialize(LogicalDevice.Object.Handle, PhysicalDevice, GraphicsQueue, FramesResources.front().CommandBuffer[0], Swapchain, DepthFormat);

      

      return true;
    }

    virtual bool Draw() override {
      
      auto prepare_frame = [&]( std::vector<VkCommandBuffer> command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
        


        
           int i = 0;
          
          if( !BeginCommandBufferRecordingOperation( command_buffer[i], VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
          return false;
          }
        
        if( true ) {

          
            UpdateUniformBuffer = false;

            BufferTransition pre_transfer_transition = {
              *board.UniformBuffer,               // VkBuffer         Buffer
              VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    CurrentAccess
              VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    NewAccess
              VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
              VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
            };
            SetBufferMemoryBarrier( command_buffer[i], VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, { pre_transfer_transition } );

            std::vector<VkBufferCopy> regions = {
              {
                0,                        // VkDeviceSize     srcOffset
                0,                        // VkDeviceSize     dstOffset
                2 * 16 * sizeof( float )  // VkDeviceSize     size
              }
            };
            CopyDataBetweenBuffers( command_buffer[i], *board.StagingBuffer, *board.UniformBuffer, regions );

            BufferTransition post_transfer_transition = {
              *board.UniformBuffer,               // VkBuffer         Buffer
              VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    CurrentAccess
              VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    NewAccess
              VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
              VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
            };
            SetBufferMemoryBarrier( command_buffer[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, { post_transfer_transition } );
          
        }

        if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
          ImageTransition image_transition_before_drawing = {
            Swapchain.Images[swapchain_image_index],  // VkImage              Image
            VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        NewAccess
            VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
            PresentQueue.FamilyIndex,                 // uint32_t             CurrentQueueFamily
            GraphicsQueue.FamilyIndex,                // uint32_t             NewQueueFamily
            VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
          };
          SetImageMemoryBarrier( command_buffer[i], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
        }

        // Drawing
        

        board.draw_1(command_buffer[i], framebuffer, {{0,0}, Swapchain.Size});

        if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
          ImageTransition image_transition_before_present = {
            Swapchain.Images[swapchain_image_index],  // VkImage              Image
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        CurrentAccess
            VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
            GraphicsQueue.FamilyIndex,                // uint32_t             CurrentQueueFamily
            PresentQueue.FamilyIndex,                 // uint32_t             NewQueueFamily
            VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
          };
          SetImageMemoryBarrier( command_buffer[i], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );
        }

        if( !EndCommandBufferRecordingOperation( command_buffer[i] ) ) {
          return false;
        }
      
        return true;
      };

      return IncreasePerformanceThroughIncreasingTheNumberOfSeparatelyRenderedFrames( *LogicalDevice, GraphicsQueue.Handle, PresentQueue.Handle,
        Swapchain.Handle.Object.Handle, Swapchain.Size, Swapchain.ImageViewsRaw, board.RenderPass.Object.Handle, {}, prepare_frame, FramesResources );
    }

    
    void OnMouseEvent()
    {
        board.UpdateStagingBuffer(true, LogicalDevice.Object.Handle, Swapchain.Size.width, Swapchain.Size.height);
    }

    

    bool Resize()
    {
        if (!CreateSwapchain()) {
            return false;
        }

        if (IsReady()) {

          board.UpdateStagingBuffer(true, LogicalDevice.Object.Handle, Swapchain.Size.width, Swapchain.Size.height);

            /*if (!UpdateStagingBuffer(true)) {
                return false;
            }*/
        }
        return true;
    }

    void Deinitialize() {

        if (LogicalDevice) {
            WaitForAllSubmittedCommandsToBeFinished(*LogicalDevice);
        }

    }

private:

};