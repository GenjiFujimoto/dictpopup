#include <stdio.h>
#include <string.h>

#include <glib.h>

#include "util.h"
#include "readsettings.h"

#define printbool(boolean) \
	boolean ? "TRUE" : "FALSE"

void
print_settings(settings *cfg)
{
	printf("[Anki]\n");
	printf("Anki deck: \"%s\"\n", cfg->deck);
	printf("Anki Notetype: \"%s\"\n", cfg->notetype);
	printf("Fields: [");
	for (int i = 0; i < cfg->num_fields; i++)
	{
		printf("\"%s\"", cfg->fieldnames[i]);
		if (i != cfg->num_fields - 1)
			printf(",");
		else
			printf("]\n");
	}
	printf("Mappings : [");
	for (int i = 0; i < cfg->num_fields; i++)
	{
		printf("%i", cfg->fieldmapping[i]);
		if (i != cfg->num_fields - 1)
			printf(",");
		else
			printf("]\n");
	}
	printf("\n");
	printf("[Behaviour]\n");
	printf("Anki support: %s\n", printbool(cfg->ankisupport));
	printf("Check if existing: %s\n", printbool(cfg->checkexisting));
	printf("Copy sentence: %s\n", printbool(cfg->copysentence));
	printf("Nuke whitespace: %s\n", printbool(cfg->nukewhitespace));
}

void
fill_anki_string(GKeyFile* kf, char **cfg_option, gboolean* ankisupport, const char* entry)
{
	if (*ankisupport)
	{
		g_autoptr(GError) error = NULL;
		char *value = g_key_file_get_string(kf, "Anki", entry, &error);
		if (value == NULL || !*value)
		{
			if (error)
				notify("WARNING: %s. Disabling Anki support.", error->message);
			else
				notify("WARNING: Missing entry \"%s\" in settings file. Disabling Anki support", entry);

			*ankisupport = 0;
		}
		else
			*cfg_option = value;
	}
}

void
fill_behaviour(GKeyFile* kf, gboolean *cfg_option, const char* entry)
{
	g_autoptr(GError) error = NULL;
	gboolean value = g_key_file_get_boolean(kf, "Behaviour", entry, &error);
	if (error != NULL)
	{
		notify("Error: %s. Setting \"%s\" to True.", error->message, entry);
		g_warning("Error: %s. Setting \"%s\" to True.", error->message, entry);
		*cfg_option = TRUE;
	}
	else
		*cfg_option = value;
}

settings*
read_user_settings()
{
	settings *cfg = malloc(sizeof(settings));

	const char* config_dir = g_get_user_config_dir();
	const gchar *config_fn = g_build_filename(config_dir, "dictpopup", "config.ini", NULL);

	GError *error = NULL;
	g_autoptr(GKeyFile) kf = g_key_file_new();
	if (!g_key_file_load_from_file(kf, config_fn, G_KEY_FILE_NONE, &error))
	{
		if (g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
		{
			notify("Could not find config file: \"%s\"", config_fn);
			g_warning("Could not find config file: \"%s\"", config_fn);
		}
		else
		{
			notify("Error opening \"%s\": %s.", config_fn, error->message);
			g_warning("Error opening \"%s\": %s.", config_fn, error->message);
		}

		g_error_free(error);
		exit(1);
	}


	fill_behaviour(kf, &(cfg->ankisupport), "AnkiSupport");
	fill_behaviour(kf, &(cfg->checkexisting), "CheckIfExists");
	fill_behaviour(kf, &(cfg->copysentence), "CopySentence");
	fill_behaviour(kf, &(cfg->nukewhitespace), "NukeWhitespace");
	fill_behaviour(kf, &(cfg->pronunciationbutton), "PronunciationButton");

	fill_anki_string(kf, &(cfg->deck), &(cfg->ankisupport), "Deck");
	fill_anki_string(kf, &(cfg->notetype), &(cfg->ankisupport), "NoteType");
	if (cfg->checkexisting && cfg->ankisupport)
	{
		char *value = g_key_file_get_string(kf, "Anki", "SearchField", &error);
		if (value == NULL || !*value)
		{
			if (error)
			{
				notify("WARNING: %s. Disabling existence searching.", error->message);
				g_warning("WARNING: %s. Disabling existence searching.", error->message);
				g_error_free(error);
				error = NULL;
			}
			else
			{
				notify("WARNING: Missing entry \"SearchField\" in settings file. Disabling existence searching.");
				g_warning("WARNING: Missing entry \"SearchField\" in settings file. Disabling existence searching.");
			}
			cfg->checkexisting = 0;
		}
		else
			cfg->searchfield = value;
	}

	// Fieldnames
	size_t num_fieldnames = 0;
	if (cfg->ankisupport)
	{
		cfg->fieldnames = g_key_file_get_string_list(kf, "Anki", "FieldNames", &num_fieldnames, &error);
		cfg->num_fields = num_fieldnames;
		if (error)
		{
			notify("WARNING: %s. Disabling Anki support.", error->message);
			cfg->ankisupport = FALSE;
			g_error_free(error);
			error = NULL;
		}
	}

	// Fieldmappoings
	size_t num_fieldmappings = 0;
	if (cfg->ankisupport)
	{
		cfg->fieldmapping = g_key_file_get_integer_list(kf, "Anki", "FieldMapping", &num_fieldmappings, &error);
		if (error)
		{
			notify("WARNING: %s. Disabling Anki support.", error->message);
			cfg->ankisupport = FALSE;
			g_error_free(error);
			error = NULL;
		}

		Stopif(num_fieldnames != num_fieldmappings, exit(1), "Error: Number of fieldnames does not match number of fieldmappings.");
	}

	return cfg;
}
