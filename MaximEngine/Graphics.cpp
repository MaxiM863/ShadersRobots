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

VkDestroyer(VkDescriptorSetLayout)      DescriptorSetLayout2;
VkDestroyer(VkDescriptorPool)           DescriptorPool2;
std::vector<VkDescriptorSet>            DescriptorSets2;

VkDestroyer( VkRenderPass )             RenderPass;

bool                                    UpdateUniformBuffer;


std::vector<Mesh>                       Model;

VkDestroyer(VkImage)                    Image;
VkDestroyer(VkDeviceMemory)             ImageMemory;
VkDestroyer(VkImageView)                ImageView;
VkDestroyer(VkSampler)                  Sampler;

VkDestroyer(VkImage)                    Image2;
VkDestroyer(VkDeviceMemory)             ImageMemory2;
VkDestroyer(VkImageView)                ImageView2;
VkDestroyer(VkSampler)                  Sampler2;

///////////////////////////////////////////////////////////////

//Text*                                 textEngine;

int                                     meshPos;

///////////////////////////////////////////////////////////////

OrbitingCamera                          Camera;

float                                   rotMax;


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

    bool Initialize(WindowParameters* window_parameters)
    {     

      UpdateUniformBuffer = true;

      

      if( !InitializeVulkan( window_parameters ) ) {
        return false;
      }

      // Render pass
        std::vector<VkAttachmentDescription> attachment_descriptions = {
            {
            0,                                                // VkAttachmentDescriptionFlags     flags
            Swapchain.Format,                                 // VkFormat                         format
            VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
            VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
            VK_ATTACHMENT_STORE_OP_STORE,                     // VkAttachmentStoreOp              storeOp
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
            VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
            VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                   // VkImageLayout                    finalLayout
            },
            {
            0,                                                // VkAttachmentDescriptionFlags     flags
            DepthFormat,                                      // VkFormat                         format
            VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
            VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
            VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              storeOp
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
            VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
            VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                    finalLayout
            }
        };

        VkAttachmentReference depth_attachment = {
            1,                                                // uint32_t                             attachment
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                        layout
        };

        std::vector<VulkanCookbook::SubpassParameters> subpass_parameters = {
            {
            VK_PIPELINE_BIND_POINT_GRAPHICS,              // VkPipelineBindPoint                  PipelineType
            {},                                           // std::vector<VkAttachmentReference>   InputAttachments
            {                                             // std::vector<VkAttachmentReference>   ColorAttachments
                {
                0,                                          // uint32_t                             attachment
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout                        layout
                }
            },
            {},                                           // std::vector<VkAttachmentReference>   ResolveAttachments
            &depth_attachment,                            // VkAttachmentReference const        * DepthStencilAttachment
            {}                                            // std::vector<uint32_t>                PreserveAttachments
            }
        };

        std::vector<VkSubpassDependency> subpass_dependencies = {
            {
            VK_SUBPASS_EXTERNAL,                            // uint32_t                   srcSubpass
            0,                                              // uint32_t                   dstSubpass
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       srcStageMask
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       dstStageMask
            VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              srcAccessMask
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              dstAccessMask
            VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
            },
            {
            0,                                              // uint32_t                   srcSubpass
            VK_SUBPASS_EXTERNAL,                            // uint32_t                   dstSubpass
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       srcStageMask
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       dstStageMask
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              srcAccessMask
            VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              dstAccessMask
            VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
            }
        };

        InitVkDestroyer( LogicalDevice, RenderPass );
        
        if( !CreateRenderPass( *LogicalDevice, attachment_descriptions, subpass_parameters, subpass_dependencies, *RenderPass ) ) {
            return false;
        }      

      //textEngine = new Text(LogicalDevice.Object.Handle, PhysicalDevice, GraphicsQueue, FramesResources.front().CommandBuffer[0], Swapchain, DepthFormat, Camera, RenderPass.Object.Handle);

      return true;
    }

    virtual bool Draw() override {
      
      auto prepare_frame = [&]( std::vector<VkCommandBuffer> command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
        
           int i = 0;
          
          if( !BeginCommandBufferRecordingOperation( command_buffer[i], VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
          return false;
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
        BeginRenderPass( command_buffer[i], *RenderPass , framebuffer, {{0,0}, Swapchain.Size}, { { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0 } }, VK_SUBPASS_CONTENTS_INLINE );

        //textEngine->draw(command_buffer[i], framebuffer, {{0,0}, Swapchain.Size});


        EndRenderPass( command_buffer[i] );



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
        Swapchain.Handle.Object.Handle, Swapchain.Size, Swapchain.ImageViewsRaw, *RenderPass, {}, prepare_frame, FramesResources );
    }

    
    void OnMouseEvent()
    {
      if( MouseState.Buttons[0].IsPressed)
      {
        float horizontal_angle = 0.0f;
        float vertical_angle = 0.0f;
        
        horizontal_angle += 0.5f * MouseState.Position.Delta.X;
        vertical_angle -= 0.5f * MouseState.Position.Delta.Y;

        if( vertical_angle > 180.0f ) {
          vertical_angle = 180.0f;
        }

        if( vertical_angle < -180.0f ) {
          vertical_angle = -180.0f;
        }

        Camera.RotateHorizontally(horizontal_angle);
        Camera.RotateVertically(vertical_angle);
      } 

      //board->UpdateStagingBuffer(true, LogicalDevice.Object.Handle, Swapchain.Size.width, Swapchain.Size.height, Camera.GetMatrix());
    }

    

    bool Resize(int w, int h)
    {

      if (!CreateSwapchain(w, h)) {
            return false;
        }
      

        if (IsReady()) {

          //board->UpdateStagingBuffer(true, LogicalDevice.Object.Handle, Swapchain.Size.width, Swapchain.Size.height, Camera.GetMatrix());

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