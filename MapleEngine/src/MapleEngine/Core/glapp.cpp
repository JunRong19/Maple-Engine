#include <pch.h>
#include <glapp.h>
#include <glhelper.h>
#include <MPL.h>
typedef enum COLOR {
	R, G, B
};

void ColorLerpBG();

GLclampf r, g, b;
COLOR currentColor = R;

GLApp::GLModel GLApp::mdl;

void GLApp::GLModel::Setup_Vao() {
	// Define vertex position and color attributes
	std::array<glm::vec2, 4> pos_vert{
		glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f),
		glm::vec2(-0.5f, 0.5f), glm::vec2(-0.5f, -0.5f)
	};
	std::array<glm::vec3, 4> color_vert{
	glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
	glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f)
	};

	GLuint vbo_hdl;
	// Create a new vbo handler.
	glCreateBuffers(1, &vbo_hdl);
	// Configure vbo size to be immutable (size cannot be changed).
	// GL_DYNAMIC_STORAGE_BIT allow data in vbo to be updated.
	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec2) * pos_vert.size() + sizeof(glm::vec3) * color_vert.size(),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	// Copy position data to vbo.
	glNamedBufferSubData(vbo_hdl, 0,
		sizeof(glm::vec2) * pos_vert.size(), pos_vert.data());
	// Copy color data to vbo.
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vert.size(),
		sizeof(glm::vec3) * color_vert.size(), color_vert.data());

	// Create a new vao.
	// Vao contains information that allow data in vbo to be connected to the shader program.
	glCreateVertexArrays(1, &vaoid);

	// Enable attrib index 8 for position data. 
	glEnableVertexArrayAttrib(vaoid, 8);
	// Set format for position attrib in vbo.
	glVertexArrayAttribFormat(vaoid, 8, 2, GL_FLOAT, GL_FALSE, 0);
	// Update vao to use binding point 3 in vbo.
	glVertexArrayVertexBuffer(vaoid, 3, vbo_hdl, 0, sizeof(glm::vec2));
	// Bind position attrib (8) to vbo binding point (3).
	glVertexArrayAttribBinding(vaoid, 8, 3);

	// Enable attrib index 8 for colour data.
	glEnableVertexArrayAttrib(vaoid, 9);
	// Set format for colour attrib in vbo.
	glVertexArrayAttribFormat(vaoid, 9, 3, GL_FLOAT, GL_FALSE, 0);
	// Update vao to use binding point 4 in vbo.
	glVertexArrayVertexBuffer(vaoid, 4, vbo_hdl, sizeof(glm::vec2) * pos_vert.size(), sizeof(glm::vec3));
	// Bind colour attrib (9) to vbo binding point (4).
	glVertexArrayAttribBinding(vaoid, 9, 4);

	// Setup ebo.
	primitive_type = GL_TRIANGLES;
	// Indices array.
	std::array<GLushort, 6> idx_vert{ 0, 1, 2, 2, 3, 0 };
	idx_elem_cnt = idx_vert.size();

	// Create new ebo.
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);

	// Copy element index data to ebo.
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vert.data()),
		GL_DYNAMIC_STORAGE_BIT);
	// Bind vao and ebo.
	glVertexArrayElementBuffer(vaoid, ebo_hdl);

	// Break link to avoid changes to vao's binding.
	glBindVertexArray(0);
}

void GLApp::GLModel::Setup_Shdrpgm() {
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	// Load vert and frag shaders.
	shdr_files.emplace_back(std::make_pair(
		GL_VERTEX_SHADER,
		"res/shaders/default.vert"));
	shdr_files.emplace_back(std::make_pair(
		GL_FRAGMENT_SHADER,
		"res/shaders/default.frag"));
	// Compile and link all shader files.
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void GLApp::GLModel::Draw() {
	// there are many shader programs initialized - here we're saying
	// which specific shader program should be used to render geometry
	shdr_pgm.Use();
	// there are many models, each with their own initialized VAO object
	// here, we're saying which VAO's state should be used to set up pipe
	glBindVertexArray(vaoid);
	// here, we're saying what primitive is to be rendered and how many
	// such primitives exist.
	// the graphics driver knows where to get the indices because the VAO
	// containing this state information has been made current ...
	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);
	// after completing the rendering, we tell the driver that VAO
	// vaoid and current shader program are no longer current
	glBindVertexArray(0);
	shdr_pgm.UnUse();
}


void GLApp::Init() {
	// Start color = red.
	r = 1.0f;

	glClearColor(r, g, b, 1.0f);

	glViewport(0, 0, GLHelper::width, GLHelper::height);

	mdl.Setup_Vao();
	mdl.Setup_Shdrpgm();

}

void GLApp::Update() {
	// Update bg color for rgb effect.
	ColorLerpBG();
	glClearColor(r, g, b, 1.0f);
}

void GLApp::Draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	mdl.Draw();
}

void GLApp::Clean_Up() {
	// empty for now
}

void ColorLerpBG() {
	// RGB lerp looping.
	switch (currentColor)
	{
	case R:
		if (g >= 1.0f) {
			r -= MPL::Time.DeltaTime();
			if (r <= 0.0f) { currentColor = G; }
		}
		else { g += MPL::Time.DeltaTime(); }
		break;
	case G:
		if (b >= 1.0f) {
			g -= MPL::Time.DeltaTime();
			if (g <= 0.0f) { currentColor = B; }
		}
		else { b += MPL::Time.DeltaTime(); }
		break;
	case B:
		if (r >= 1.0f) {
			b -= MPL::Time.DeltaTime();
			if (b <= 0.0f) { currentColor = R; }
		}
		else { r += MPL::Time.DeltaTime(); }
		break;
	default:
		break;
	}
}
